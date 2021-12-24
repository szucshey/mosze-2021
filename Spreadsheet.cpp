#include "Spreadsheet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <cfloat>

bool sortnumericbyfirst(const std::pair<double, Cell>& a, const std::pair<double, Cell>& b)
{
	return a.first < b.first;
}

bool sorttextbyfirst(const std::pair<std::string, Cell>& a, const std::pair<std::string, Cell>& b)
{
	return a.first < b.first;
}

Spreadsheet::Spreadsheet(int rows, int cols, std::string name) : rows(rows + 1), cols(cols + 1), name(name) {
	Spreadsheet::createVectorSkeleton(Spreadsheet::getRows(), Spreadsheet::getCols());
}

Spreadsheet::~Spreadsheet() {}

int Spreadsheet::getRows() {
	return rows;
}

void Spreadsheet::setRows(int newrows) {
	rows = newrows;
}

int Spreadsheet::getCols() {
	if (cols > 27) {
		return 27;
	}
	else {
		return cols;
	}

}

int Spreadsheet::getColWidth(int col) {
	int colWidth = 0;
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect[i][col]->getDisplayedValue().size() > colWidth)
		{
			colWidth = vect[i][col]->getDisplayedValue().size();
		}
	}
	return colWidth;
}

void Spreadsheet::setCols(int newcols) {
	if (newcols > 27) {
		cols = 27;
	}
	else {
		cols = newcols;
	}
}

void Spreadsheet::print() {
	Spreadsheet::evaluateFunctions();
	Spreadsheet::setDisplayedValues();
	std::cout << "\n\n";
	int rowLength;
	for (int i = 0; i < vect.size(); i++)
	{
		rowLength = 0;
		for (int j = 0; j < vect[i].size(); j++)
		{
			if (vect[i][j]->getAlignment() == true)
			{
				for (int k = 0; k < Spreadsheet::getColWidth(j) - vect[i][j]->getDisplayedValue().length(); k++)
				{
					std::cout << " ";
					rowLength++;
				}
				std::cout << vect[i][j]->getDisplayedValue();
				rowLength = rowLength + vect[i][j]->getDisplayedValue().length();
				std::cout << "|";
				rowLength++;
			}
			else {
				std::cout << vect[i][j]->getDisplayedValue();
				rowLength = rowLength + vect[i][j]->getDisplayedValue().length();

				for (int k = 0; k < Spreadsheet::getColWidth(j) - vect[i][j]->getDisplayedValue().length(); k++)
				{
					std::cout << " ";
					rowLength++;
				}
				std::cout << "|";
				rowLength++;
			}
		}

		std::cout << std::endl;
		for (int j = 0; j < rowLength; j++)
		{
			std::cout << "-";
		}
		std::cout << std::endl;
	}
}

void Spreadsheet::createVectorSkeleton(int rows, int cols) {
	vect.resize(rows);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (i == 0) {
				if (j == 0) {
					vect[i].push_back(new Cell(" ", false, false, 0.0));
					vect[i][j]->setValue(" ");
					vect[i][j]->setDisplayedValue(vect[i][j]->getValue());
				}
				else {
					char c = 64 + j;
					vect[i].push_back(new Cell(std::string(1, c), false, false, 0.0));
					vect[i][j]->setValue(std::string(1, c));
					vect[i][j]->setDisplayedValue(vect[i][j]->getValue());
				}
			}
			else {
				if (j == 0) {
					vect[i].push_back(new Cell(std::to_string(i), false, false, 0.0));
					vect[i][j]->setValue(std::to_string(i));
					vect[i][j]->setDisplayedValue(vect[i][j]->getValue());
				}
				else {
					vect[i].push_back(new Cell(" ", false, false, 0.0));
					vect[i][j]->setValue(" ");
					vect[i][j]->setDisplayedValue(vect[i][j]->getValue());
				}
			}
		}
	}
	Spreadsheet::setCellCoords();
}

void Spreadsheet::fixVectorSkeleton() {
	for (int i = 1; i < vect[0].size(); i++) {
		char c = 64 + i;
		vect[0][i]->setValue(std::string(1, c));
		vect[0][i]->setDisplayedValue(vect[0][i]->getValue());
	}

	for (int i = 1; i < vect.size(); i++) {
		vect[i][0]->setValue(std::to_string(i));
		vect[i][0]->setDisplayedValue(vect[i][0]->getValue());
	}
	Spreadsheet::setCellCoords();
}

void Spreadsheet::edit(int row, int col, std::string value) {
	if (row >= Spreadsheet::getRows() || col >= Spreadsheet::getCols()) {
		std::cout << "Hibas koordinata!" << std::endl;
	}
	else {
		vect[row][col]->setValue(value);
	}
	if (vect[row][col]->getValue().substr(0, 5) == "=SUM(" || vect[row][col]->getValue().substr(0, 5) == "=AVG(" ||
		vect[row][col]->getValue().substr(0, 5) == "=MIN(" || vect[row][col]->getValue().substr(0, 5) == "=MAX(")
	{
		vect[row][col]->setFunctionMode(true);
	}
}

void Spreadsheet::add(int num, bool dimension) {
	if (dimension == 0) {

		for (int i = 0; i < vect.size(); i++) {
			for (int j = 0; j < num; j++)
			{
				vect[i].push_back(new Cell(" ", false, false, 0.0));
				vect[i].back()->setValue(" ");
				vect[i].back()->setDisplayedValue(vect[i].back()->getValue());
			}
		}

		Spreadsheet::setCols(Spreadsheet::getCols() + num);
		Spreadsheet::fixVectorSkeleton();

	}
	else {
		vect.resize(vect.size() + num);

		for (int i = vect.size() - num; i < vect.size(); i++) {
			for (int j = 0; j < vect[i - 1].size(); j++)
			{
				vect[i].push_back(new Cell(" ", false, false, 0.0));
				vect[i].back()->setValue(" ");
				vect[i].back()->setDisplayedValue(vect[i].back()->getValue());
			}
		}

		Spreadsheet::setRows(Spreadsheet::getRows() + num);
		Spreadsheet::fixVectorSkeleton();
	}
}

void Spreadsheet::remove(std::string toDelete, bool isLetter) {
	if (isLetter) {
		if (Spreadsheet::getCols() == 2) {
			std::cout << "Nem torolheto az oszlop!" << std::endl;
		}
		else {
			char ch = toDelete[0];
			if (ch < 65 || ch > Spreadsheet::getCols() + 63) {
				std::cout << "Hibasan adtad meg a torlendo oszlopot!" << std::endl;
			}
			else {
				for (int i = 0; i < vect.size(); i++) {
					delete vect[i][(int(ch) - 64)];
					vect[i].erase(vect[i].begin() + (int(ch) - 64));
				}
				Spreadsheet::fixVectorSkeleton();
				Spreadsheet::setCols(Spreadsheet::getCols() - 1);
			}
		}
	}

	else {
		if (Spreadsheet::getRows() == 2) {
			std::cout << "Nem torolheto a sor!" << std::endl;
		}
		else {
			if (std::stoi(toDelete) < 1 || std::stoi(toDelete) > Spreadsheet::getRows() - 1) {
				std::cout << "Hibasan adtad meg a torlendo sorszamot!" << std::endl;
			}
			else {
				for (int i = 0; i < vect[std::stoi(toDelete)].size(); i++)
				{
					delete vect[std::stoi(toDelete)][i];
				}
				vect.erase(vect.begin() + std::stoi(toDelete));
				Spreadsheet::fixVectorSkeleton();
				Spreadsheet::setRows(Spreadsheet::getRows() - 1);
			}
		}
	}
}

void Spreadsheet::insert(int numToInsert, int insertInto, bool isLetter) {
	if (isLetter)
	{
		for (int i = 0; i < vect.size(); i++)
		{
			for (int j = 0; j < numToInsert; j++)
			{
				vect[i].insert(vect[i].begin() + insertInto, new Cell(" ", false, false, 0.0));
				vect[i][insertInto]->setDisplayedValue(" ");
			}
		}
		Spreadsheet::fixVectorSkeleton();
		Spreadsheet::setCols(Spreadsheet::getCols() + numToInsert);
	}
	else {
		std::vector<Cell*> insertedRow;
		for (int i = 0; i < numToInsert; i++) {
			for (int j = 0; j < vect[i].size(); j++) {
				insertedRow.push_back(new Cell(" ", false, false, 0.0));
				insertedRow.back()->setDisplayedValue(insertedRow.back()->getValue());
			}
			vect.insert(vect.begin() + insertInto, insertedRow);
			insertedRow.clear();
		}
		Spreadsheet::fixVectorSkeleton();
		Spreadsheet::setRows(Spreadsheet::getRows() + numToInsert);
	}
}

void Spreadsheet::swap(int row1, int row2, int col1, int col2) {
	Cell temp = *vect[row1][col1];

	temp.setRow(row1);
	temp.setCol(col1);
	temp.setValue(vect[row1][col1]->getValue());
	temp.setFunctionMode(vect[row1][col1]->getFunctionMode());
	temp.setFunctionValue(vect[row1][col1]->getFunctionValue());
	temp.setErrValue(vect[row1][col1]->getErrValue());
	temp.setErrorInFunction(vect[row1][col1]->getErrorInFunction());
	temp.setDisplayedValue(vect[row1][col1]->getDisplayedValue());
	for (int i = 0; i < vect[row1][col1]->getLinkedCells().size(); i++)
	{
		temp.setLinkedCell(vect[row1][col1]->getLinkedCells()[i]);
	}

	vect[row1][col1]->setValue(vect[row2][col2]->getValue());
	vect[row1][col1]->setFunctionMode(vect[row2][col2]->getFunctionMode());
	vect[row1][col1]->setFunctionValue(vect[row2][col2]->getFunctionValue());
	vect[row1][col1]->setErrValue(vect[row2][col2]->getErrValue());
	vect[row1][col1]->setErrorInFunction(vect[row2][col2]->getErrorInFunction());
	vect[row1][col1]->setDisplayedValue(vect[row2][col2]->getDisplayedValue());
	vect[row1][col1]->clearLinkedCells();

	for (int i = 0; i < vect[row2][col2]->getLinkedCells().size(); i++)
	{
		vect[row1][col1]->setLinkedCell(vect[row2][col2]->getLinkedCells()[i]);
	}

	vect[row2][col2]->setValue(temp.getValue());
	vect[row2][col2]->setFunctionMode(temp.getFunctionMode());
	vect[row2][col2]->setFunctionValue(temp.getFunctionValue());
	vect[row2][col2]->setErrValue(temp.getErrValue());
	vect[row2][col2]->setErrorInFunction(temp.getErrorInFunction());
	vect[row2][col2]->setDisplayedValue(temp.getDisplayedValue());
	vect[row2][col2]->clearLinkedCells();

	for (int i = 0; i < temp.getLinkedCells().size(); i++)
	{
		vect[row2][col2]->setLinkedCell(temp.getLinkedCells()[i]);
	}
}

void Spreadsheet::sort(int num, bool isColumn, bool order) {
	std::vector<std::pair <double, Cell>> numericCells;
	std::vector<std::pair<std::string, Cell>> textCells;
	std::vector<std::pair<std::string, Cell>> sortingVect;
	bool isNumber;
	if (isColumn) {
		for (int i = 1; i < vect.size(); i++) {
			isNumber = true;
			for (auto j : vect[i][num]->getDisplayedValue()) {
				if (isalpha(j) || vect[i][num]->getDisplayedValue() == " " || vect[i][num]->getDisplayedValue() == "") {
					isNumber = false;
					break;
				}
			}
			if (isNumber) {
				numericCells.push_back(std::make_pair(std::stof(vect[i][num]->getDisplayedValue()), *vect[i][num]));
			}
			else {
				textCells.push_back(std::make_pair(vect[i][num]->getDisplayedValue(), *vect[i][num]));
			}
		}

		if (order) {

			std::sort(textCells.begin(), textCells.end(), sorttextbyfirst);
			std::sort(numericCells.begin(), numericCells.end(), sortnumericbyfirst);

			for (auto i : textCells) {
				sortingVect.push_back(i);
			}
			for (auto i : numericCells) {
				sortingVect.push_back(std::make_pair(std::to_string(i.first), i.second));
			}

		}
		else {

			std::sort(textCells.rbegin(), textCells.rend(), sorttextbyfirst);
			std::sort(numericCells.rbegin(), numericCells.rend(), sortnumericbyfirst);

			for (auto i : numericCells) {
				sortingVect.push_back(std::make_pair(std::to_string(i.first), i.second));
			}
			for (auto i : textCells) {
				sortingVect.push_back(i);
			}

		}
		for (int i = 0; i < sortingVect.size(); i++) {
			vect[i + 1][num]->setDisplayedValue(sortingVect[i].second.getDisplayedValue());
			vect[i + 1][num]->setValue(sortingVect[i].second.getValue());
			vect[i + 1][num]->setFunctionMode(sortingVect[i].second.getFunctionMode());
			vect[i + 1][num]->setFunctionValue(sortingVect[i].second.getFunctionValue());
			vect[i + 1][num]->setErrorInFunction(sortingVect[i].second.getErrorInFunction());
			vect[i + 1][num]->setErrValue(sortingVect[i].second.getErrValue());
			vect[i + 1][num]->clearLinkedCells();
			for (int j = 0; j < sortingVect[i].second.getLinkedCells().size(); j++)
			{
				vect[i + 1][num]->setLinkedCell(sortingVect[i].second.getLinkedCells()[j]);
			}

		}
	}
	else {
		for (int i = 1; i < vect[num].size(); i++) {
			isNumber = true;
			for (auto j : vect[num][i]->getDisplayedValue()) {
				if (isalpha(j) || j == ' ') {
					isNumber = false;
					break;
				}
			}
			if (isNumber) {
				numericCells.push_back(std::make_pair(std::stof(vect[num][i]->getDisplayedValue()), *vect[num][i]));
			}
			else {
				textCells.push_back(std::make_pair(vect[num][i]->getDisplayedValue(), *vect[num][i]));
			}
		}

		if (order) {
			std::sort(textCells.begin(), textCells.end(), sorttextbyfirst);
			std::sort(numericCells.begin(), numericCells.end(), sortnumericbyfirst);
			for (auto i : textCells) {
				sortingVect.push_back(i);
			}
			for (auto i : numericCells) {
				sortingVect.push_back(std::make_pair(std::to_string(i.first), i.second));
			}

		}
		else {
			std::sort(textCells.rbegin(), textCells.rend(), sorttextbyfirst);
			std::sort(numericCells.rbegin(), numericCells.rend(), sortnumericbyfirst);
			for (auto i : numericCells) {
				sortingVect.push_back(std::make_pair(std::to_string(i.first), i.second));
			}
			for (auto i : textCells) {
				sortingVect.push_back(i);
			}

		}
		for (int i = 0; i < sortingVect.size(); i++) {
			vect[num][i + 1]->setDisplayedValue(sortingVect[i].second.getDisplayedValue());
			vect[num][i + 1]->setValue(sortingVect[i].second.getValue());
			vect[num][i + 1]->setFunctionMode(sortingVect[i].second.getFunctionMode());
			vect[num][i + 1]->setFunctionValue(sortingVect[i].second.getFunctionValue());
			vect[num][i + 1]->setErrorInFunction(sortingVect[i].second.getErrorInFunction());
			vect[num][i + 1]->setErrValue(sortingVect[i].second.getErrValue());
			vect[num][i + 1]->clearLinkedCells();
			for (int j = 0; j < sortingVect[i].second.getLinkedCells().size(); j++)
			{
				vect[num][i + 1]->setLinkedCell(sortingVect[i].second.getLinkedCells()[j]);
			}

		}
	}
}

void Spreadsheet::align(int row, int col, bool alignment) {
	vect[row][col]->setAlignment(alignment);
}

void Spreadsheet::clear(int row, int col) {
	vect[row][col]->setValue("");
	vect[row][col]->setFunctionValue(0.0);
	vect[row][col]->setFunctionMode(false);
	vect[row][col]->setErrorInFunction(false);
	vect[row][col]->setErrValue("");
	vect[row][col]->setDisplayedValue("");
	vect[row][col]->clearLinkedCells();
	for (int i = 0; i < functions.size(); i++)
	{
		if (functions[i].getRow() == row && functions[i].getCol() == col) {
			functions.erase(functions.begin() + i);
		}
	}
}

void Spreadsheet::fillVectorFromFile(std::vector<std::vector<std::string>> csv) {
	for (int i = 0; i < csv.size(); i++)
	{
		for (int j = 0; j < csv[i].size(); j++)
		{
			Spreadsheet::edit(i + 1, j + 1, csv[i][j]);
		}
	}
}

void Spreadsheet::saveToCSV(std::string filename, std::string delimiter) {
	std::ofstream csvFile;
	csvFile.open(filename);
	for (int i = 1; i < vect.size(); i++)
	{
		for (int j = 1; j < vect[i].size(); j++)
		{
			if (vect[i][j]->getValue() != " " && vect[i][j]->getValue() != "")
			{
				csvFile << vect[i][j]->getValue();
				if (j != vect[i].size() - 1)
				{
					csvFile << delimiter;
				}
			}
			else {
				csvFile << " ";
				if (j != vect[i].size() - 1)
				{
					csvFile << delimiter;
				}
			}
		}
		csvFile << "\n";
	}
	csvFile.close();
}

void Spreadsheet::setCellCoords() {
	for (int i = 1; i < vect.size(); i++)
	{
		for (int j = 1; j < vect[i].size(); j++)
		{
			vect[i][j]->setRow(i);
			vect[i][j]->setCol(j);
		}
	}
}

bool Spreadsheet::functionInputCheck(std::string value) {
	std::string coord1, coord2;
	int ul_row, ul_col;
	int lr_row, lr_col;

	coord1 = value.substr(5, value.find(":") - 5);
	std::transform(coord1.begin(), coord1.end(), coord1.begin(), [](unsigned char c) { return std::toupper(c); });

	coord2 = value.substr(value.find(":") + 1, value.length() - value.find(":") - 2);
	std::transform(coord2.begin(), coord2.end(), coord2.begin(), [](unsigned char c) { return std::toupper(c); });

	ul_col = std::min(coord1[0], coord2[0]) - 64;
	lr_col = std::max(coord1[0], coord2[0]) - 64;

	ul_row = std::min(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));
	lr_row = std::max(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));

	if (ul_col > Spreadsheet::getCols() - 1 || lr_col > Spreadsheet::getCols() - 1)
	{
		return false;
	}
	else if (ul_row > Spreadsheet::getRows() - 1 || lr_row > Spreadsheet::getRows() - 1)
	{
		return false;
	}
	else {
		return true;
	}
}

void Spreadsheet::evaluateFunctions() {
	std::string coord1, coord2;
	int ul_row, ul_col;
	int lr_row, lr_col;
	double result;

	Spreadsheet::findFunctions();
	for (int i = 0; i < functions.size(); i++)
	{
		vect[functions[i].getRow()][functions[i].getCol()]->clearLinkedCells();
		functions[i].clearLinkedCells();
		result = 0;
		if (functionInputCheck(functions[i].getValue()) == true)
		{
			vect[functions[i].getRow()][functions[i].getCol()]->setErrorInFunction(false);
			vect[functions[i].getRow()][functions[i].getCol()]->setErrValue("");
			coord1 = functions[i].getValue().substr(5, functions[i].getValue().find(":") - 5);
			std::transform(coord1.begin(), coord1.end(), coord1.begin(), [](unsigned char c) { return std::toupper(c); });

			coord2 = functions[i].getValue().substr(functions[i].getValue().find(":") + 1, functions[i].getValue().length() - (functions[i].getValue().find(":") + 2));
			std::transform(coord2.begin(), coord2.end(), coord2.begin(), [](unsigned char c) { return std::toupper(c); });

			ul_col = std::min(coord1[0], coord2[0]) - 64;
			lr_col = std::max(coord1[0], coord2[0]) - 64;

			ul_row = std::min(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));
			lr_row = std::max(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));

			if (vect[functions[i].getRow()][functions[i].getCol()]->getValue().substr(0, 5) == "=SUM(")
			{
				for (int k = ul_row; k <= lr_row; k++)
				{
					for (int l = ul_col; l <= lr_col; l++)
					{
						if (vect[k][l]->getFunctionMode() == true && vect[k][l]->getErrorInFunction() == false)
						{
							result = result + vect[k][l]->getFunctionValue();
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
						else {

							for (auto c : vect[k][l]->getValue())
							{
								if (isalpha(c) || isblank(c) || c == ' ')
								{
									break;
								}
							}

							try
							{
								result = result + std::stof(vect[k][l]->getValue());
							}
							catch (const std::exception & e) {}
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
					}
				}
				vect[functions[i].getRow()][functions[i].getCol()]->setFunctionValue(result);
			}
			else if (vect[functions[i].getRow()][functions[i].getCol()]->getValue().substr(0, 5) == "=AVG(")
			{
				int n = 0;
				double sum = 0;
				for (int k = ul_row; k <= lr_row; k++)
				{
					for (int l = ul_col; l <= lr_col; l++)
					{
						if (vect[k][l]->getFunctionMode() == true && vect[k][l]->getErrorInFunction() == false)
						{
							sum = sum + vect[k][l]->getFunctionValue();
							n++;
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
						else {

							for (auto c : vect[k][l]->getValue())
							{
								if (isalpha(c) || isblank(c) || c == ' ')
								{
									break;
								}
							}
							try
							{
								sum = sum + std::stof(vect[k][l]->getValue());
								n++;
							}
							catch (const std::exception & e) {}
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
					}
				}
				result = sum / n;
				vect[functions[i].getRow()][functions[i].getCol()]->setFunctionValue(result);
			}
			else if (vect[functions[i].getRow()][functions[i].getCol()]->getValue().substr(0, 5) == "=MIN(")
			{
				result = DBL_MAX;
				for (int k = ul_row; k <= lr_row; k++)
				{
					for (int l = ul_col; l <= lr_col; l++)
					{
						if (vect[k][l]->getFunctionMode() == true && vect[k][l]->getErrorInFunction() == false)
						{
							if (vect[k][l]->getFunctionValue() < result)
							{
								result = vect[k][l]->getFunctionValue();
							}
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
						else {

							for (auto c : vect[k][l]->getValue())
							{
								if (isalpha(c) || isblank(c) || c == ' ')
								{
									break;
								}
							}

							try
							{
								if (std::stof(vect[k][l]->getValue()) < result)
								{
									result = std::stof(vect[k][l]->getValue());
								}
							}
							catch (const std::exception & e) {}
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
					}
				}
				vect[functions[i].getRow()][functions[i].getCol()]->setFunctionValue(result);
			}
			else if (vect[functions[i].getRow()][functions[i].getCol()]->getValue().substr(0, 5) == "=MAX(")
			{
				result = DBL_MIN;
				for (int k = ul_row; k <= lr_row; k++)
				{
					for (int l = ul_col; l <= lr_col; l++)
					{
						if (vect[k][l]->getFunctionMode() == true && vect[k][l]->getErrorInFunction() == false)
						{
							if (vect[k][l]->getFunctionValue() > result)
							{
								result = vect[k][l]->getFunctionValue();
							}
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
						else {

							for (auto c : vect[k][l]->getValue())
							{
								if (isalpha(c) || isblank(c) || c == ' ')
								{
									break;
								}
							}

							try
							{
								if (std::stof(vect[k][l]->getValue()) > result)
								{
									result = std::stof(vect[k][l]->getValue());
								}
							}
							catch (const std::exception & e) {}
							vect[functions[i].getRow()][functions[i].getCol()]->setLinkedCell(*vect[k][l]);
							functions[i].setLinkedCell(*vect[k][l]);
						}
					}
				}
				vect[functions[i].getRow()][functions[i].getCol()]->setFunctionValue(result);
			}
		}
		else {
			vect[functions[i].getRow()][functions[i].getCol()]->setErrValue("#NAME?");
			vect[functions[i].getRow()][functions[i].getCol()]->setErrorInFunction(true);
		}
	}
	Spreadsheet::checkCircularFunctions(functions);
}

void Spreadsheet::checkCircularFunctions(std::vector<Cell> functions) {
	for (auto i : functions)
	{
		for (auto cell : i.getLinkedCells())
		{
			for (auto j : functions)
			{
				if (cell.getRow() == j.getRow() && cell.getCol() == j.getCol())
				{
					for (auto cell2 : j.getLinkedCells()) {

						if (cell2.getRow() == i.getRow() && cell2.getCol() == i.getCol())
						{
							vect[i.getRow()][i.getCol()]->setErrorInFunction(true);
							vect[i.getRow()][i.getCol()]->setErrValue("#CYCLE?");

							vect[j.getRow()][j.getCol()]->setErrorInFunction(true);
							vect[j.getRow()][j.getCol()]->setErrValue("#CYCLE?");
						}
					}
				}
			}
		}
	}
}

void Spreadsheet::findFunctions() {
	functions.clear();
	for (int i = 1; i < vect.size(); i++)
	{
		for (int j = 1; j < vect[i].size(); j++)
		{
			if (vect[i][j]->getValue().substr(0, 5) == "=SUM(" || vect[i][j]->getValue().substr(0, 5) == "=AVG(" ||
				vect[i][j]->getValue().substr(0, 5) == "=MIN(" || vect[i][j]->getValue().substr(0, 5) == "=MAX(")
			{
				functions.push_back(*vect[i][j]);
				vect[i][j]->setFunctionMode(true);
			}
		}
	}
};

std::vector<std::vector<Cell*>> Spreadsheet::getVector() {
	return vect;
}

void Spreadsheet::setDisplayedValues() {
	for (int i = 1; i < vect.size(); i++)
	{
		for (int j = 1; j < vect[i].size(); j++) {
			if (vect[i][j]->getFunctionMode() == true)
			{
				if (vect[i][j]->getErrorInFunction() == true) {
					vect[i][j]->setDisplayedValue(vect[i][j]->getErrValue());
				}
				else {
					std::stringstream stream;
					stream << std::fixed << std::setprecision(3) << vect[i][j]->getFunctionValue();
					vect[i][j]->setDisplayedValue(stream.str());
				}
			}
			else {
				vect[i][j]->setDisplayedValue(vect[i][j]->getValue());
			}
		}
	}
}

void Spreadsheet::renderChart(int ul_row, int ul_col, int lr_row, int lr_col, std::string file) {
	std::ofstream svg;
	double h = 450;
	int xCounter = 1;
	double yCounter = 0;
	double min = DBL_MAX;
	double max = DBL_MIN;
	//kezdeti ellenorzesek
	for (int i = ul_row + 1; i <= lr_row; i++)
	{
		for (int j = ul_col + 1; j <= lr_col; j++)
		{
			for (auto ch : vect[i][j]->getDisplayedValue())
			{
				if (isalpha(ch))
				{
					std::cout << "Hibas ertek a(z) " << char(64 + j) << i << " cellaban!\n";
					return;
				}
			}

			if (vect[i][j]->getDisplayedValue() == " " || vect[i][j]->getDisplayedValue() == "")
			{
				std::cout << "Hiba! A(z) " << char(64 + j) << i << " cella ures!\n";
				return;
			}


			if (std::stof(vect[i][j]->getDisplayedValue()) > max)
			{
				max = std::stof(vect[i][j]->getDisplayedValue());
			}
			if (std::stof(vect[i][j]->getDisplayedValue()) < min)
			{
				min = std::stof(vect[i][j]->getDisplayedValue());
			}
		}
	}

	//svg fajl megnyitasa
	svg.open(file + ".svg");
	svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"960\" height=\"600\">\n";
	svg << "\t<g transform=\"translate(50,50)\">\n";
	svg << "\t\t<g class = \"x-axis\" transform=\"translate(0,450)\">\n";

	//x-tengely felteltese az elso oszlop tagjaival
	for (int i = ul_row + 1; i <= lr_row; i++)
	{
		svg << "\t\t\t<g class=\"tick\" transform=\"translate(" << xCounter * double((910 / (lr_row - ul_row + 1))) << ",0)\" style=\"opacity: 1;\">\n";
		svg << "\t\t\t\t<line y2=\"6\" x2=\"0\"></line>\n";
		svg << "\t\t\t\t<text dy=\".71em\" y=\"9\" x=\"0\" style=\"text-anchor: middle;\">";
		svg << vect[i][ul_col]->getDisplayedValue();
		svg << "</text>\n";
		svg << "\t\t\t</g>\n";
		xCounter++;
	}
	svg << "\t\t<line x1=\"0\" y1=\"-5\" x2=\"910\" y2=\"-5\" stroke=\"black\" stroke-width=\"2\"></line>\n";
	svg << "\t\t</g>\n";
	svg << "\t\t<g class = \"y-axis\" transform=\"translate(0,-20)\">\n";

	//y-tengely feltoltese 0-tol a legnagyobb ertekig tiz fokra skalazva
	while (h >= 0)
	{
		svg << "\t\t\t<g class=\"tick\" transform=\"translate(0," << h + 15 << ")\" style=\"opacity: 1;\">\n";
		svg << "\t\t\t\t<line x2=\"-6\" y2=\"0\"></line>\n";
		svg << "\t\t\t\t<text dy=\".32em\" x=\"-9\" y=\"0\" style=\"text-anchor: end;\">";
		svg << yCounter << "</text>\n";
		svg << "\t\t\t</g>\n";
		svg << "\t\t<line x1=\"0\" y1=\"" << h + 15 << "\" x2=\"910\" y2=\"" << h + 15 << "\" stroke=\"lightgray\" stroke-width=\"2\"></line>\n";
		h = h - 45;
		yCounter = yCounter + max / 10;
	}
	svg << "\t\t<line x1=\"0\" y1=\"-5\" x2=\"0\" y2=\"465\" stroke=\"black\" stroke-width=\"2\"></line>\n";
	svg << "\t\t</g>\n";

	int x = 0;
	int w = 0;
	int height = 0;
	int red = 0;
	int blue = 0;
	int green = 0;
	int columns = lr_col - ul_col;
	int rows = lr_row - ul_row + 1;
	w = 128 / (cols + 1);

	//az egyes ertekekhez rendelt oszlopok letrehozasa
	for (int i = ul_col + 1; i <= lr_col; i++)
	{
		//minden kategorianak kulon szinkodja lesz
		red = red + 207;
		green = green + 57;
		blue = blue + 141;

		//az oszlopok elhelyezesere a keplet (jobb megoldas erre esetleg?)
		x = 455 - double((910 / (lr_row - ul_row + 1))) + (((lr_row - ul_row - 1) / 2) * (w + 5)) - (i - (ul_row + 1)) * (w + 5);

		//az oszlopok elhelyezese
		for (int j = ul_row + 1; j <= lr_row; j++)
		{
			height = std::stof(vect[j][i]->getDisplayedValue()) / max * 450;
			svg << "\t\t<rect class=\"bar\" x=\"" << x << "\" width=\"" << w << "\" y=\"206\" height=\"" << height << "\" transform=\"rotate(180 227 326)\" fill=\"rgb(" << red % 255 << "," << green % 255 << "," << blue % 255 << ")\"></rect>\n";
			x = x - double((885 / (lr_row - ul_row + 1)));
		}

		//data label-ek
		svg << "\t\t<g class=\"data-labels\" transform=\"translate(" << (i - (ul_col + 1)) * (910 / ((lr_col - ul_col) + 1)) << ",525)\" style=\"opacity: 1;\">\n";
		svg << "\t\t\t<rect class=\"rect\" x=\"0\" width=\"20\" y=\"0\" height=\"20\" fill=\"rgb(" << red % 255 << "," << green % 255 << "," << blue % 255 << ")\"></rect>\n";
		svg << "\t\t\t<line y2=\"6\" x2=\"0\"></line>\n";
		svg << "\t\t\t<text dy=\".71em\" y=\"5\" x=\"" << 45 + vect[ul_col][i]->getDisplayedValue().length() * 2 << "\" style=\"text-anchor: middle;\">";
		svg << vect[ul_col][i]->getDisplayedValue();
		svg << "</text>\n";
		svg << "\t\t</g>\n";
	}
	svg << "\t</g>\n";
	svg << "</svg>";
	svg.close();
}

std::string Spreadsheet::getName()
{
	return name;
}

void Spreadsheet::setName(std::string name_)
{
	name = name_;
}

int Spreadsheet::getID()
{
	return id;
}

void Spreadsheet::setID(int id_)
{
	id = id_;
}
