#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Spreadsheet.h"

int main(int argc, char** argv);

std::vector<std::string> tokenize(std::string, int, std::vector<std::string>);
std::vector<std::vector<std::string>> readCSV(std::string, std::string);
bool checkCLIArguments(int, char**);
int getInputWordCount(std::string);
void printSheets(std::vector<Spreadsheet*>, Spreadsheet*);

int main(int argc, char** argv) {

	std::string input;
	std::vector<std::string> tokens;
	std::vector<std::vector<std::string>> csvToVector;
	std::string file;
	std::string delimiter = ";";
	int maxLength = 0;
	int words;
	Spreadsheet* activesheet;
	std::vector<Spreadsheet*> sheets;

	if (argc == 1) {
		activesheet = new Spreadsheet(1, 1);
		activesheet->setID(0);
		sheets.push_back(activesheet);
	}
	else {
		if (checkCLIArguments(argc, argv) == 0) {
			file = argv[1];
			if (argc == 4)
			{
				delimiter = argv[3];
			}
			csvToVector = readCSV(file, delimiter);
			for (auto i : csvToVector)
			{
				if (i.size() > maxLength)
				{
					maxLength = i.size();
				}
			}
			activesheet = new Spreadsheet(csvToVector.size(), maxLength, file.substr(0, file.find(".csv")));
			activesheet->fillVectorFromFile(csvToVector);
			activesheet->setID(0);
			sheets.push_back(activesheet);
		}
		else {
			return 0;
		}
	}
	activesheet->print();
	std::cout << activesheet->getID() << "*: " << activesheet->getName();
	std::cout << "\n\n";

	while (std::getline(std::cin, input)) {
		if (!input.empty() && int(input[input.length() - 1]) == 13) {
			input.pop_back();
		}
		words = getInputWordCount(input);

		if (input.substr(0, 4) == "edit") {
			//edit esetén az input minimum 3 szóból kell álljon, kevesebb szó esetén reklamálunk
			if (words < 3) {
				std::cout << "Hianyos parameterek! Az edit parancs szintaxisa: edit <cella> <ertek>" << std::endl;
			}
			else {
				int rowToEdit;
				int colToEdit;

				//az input feldarabolása tokenekre: <edit>, <mező>, <minden más ami megmaradt az inputban>
				tokens = tokenize(input, words, tokens);

				std::transform(tokens.at(1).begin(), tokens.at(1).end(), tokens.at(1).begin(), [](unsigned char c) { return std::toupper(c); });

				//a megadott cellából kinyerjük, hogy melyik cellát kell módosítani
				int charVal = tokens.at(1).at(0);

				if (isalpha(tokens.at(1)[1]))
				{
					std::cout << "Hibas koordinata!" << std::endl;
				}

				else {
					rowToEdit = (std::stoi(tokens.at(1).substr(1, tokens.at(1).size() - 1)) < 1) ? 1 : std::stoi(tokens.at(1).substr(1, tokens.at(1).size() - 1));
					colToEdit = (charVal - 64 < 1) ? 1 : charVal - 64;
					activesheet->edit(rowToEdit, colToEdit, tokens[2]);
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 3) == "add")
		{
			//add esetén az input szigorúan 3 szóból kell álljon, ellenkező esetben reklamálunk
			if (words != 3) {
				std::cout << "Hibas parameterek! Az add parancs szintaxisa: add <darabszam> <rows/cols>" << std::endl;
			}
			else {
				int numToAdd;
				int currentCols;
				bool isNumber = true;
				tokens = tokenize(input, words, tokens);
				for (auto i : tokens[1])
				{
					if (isdigit(int(i)) == false)
					{
						std::cout << "Hibas parameter! Szamot adj meg!" << std::endl;
						isNumber = false;
						break;
					}
				}

				if (isNumber)
				{
					numToAdd = std::stoi(tokens[1]);

					if (tokens.back() == "cols") {
						currentCols = activesheet->getCols();
						if (currentCols + numToAdd > 27) {
							numToAdd = 27 - currentCols;
						}
						activesheet->add(numToAdd, 0);
					}
					else if (tokens.back() == "rows") {
						activesheet->add(numToAdd, 1);
					}
					else {
						std::cout << "Hibas az utolso parameter! Az add parancs szintaxisa: add <darabszam> <rows/cols>" << std::endl;
					}
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 6) == "delete") {
			//delete esetén az input szigorúan 2 szóból kell álljon, ellenkező esetben reklamálunk
			if (words != 2) {
				std::cout << "Hibas parameterek! Az add parancs szintaxisa: add <darabszam> <rows/cols>" << std::endl;
			}
			else {
				std::string toDelete;
				bool isLetter = false;

				tokens = tokenize(input, words, tokens);

				std::transform(tokens.back().begin(), tokens.back().end(), tokens.back().begin(), [](unsigned char c) { return std::toupper(c); });
				toDelete = tokens.back();

				for (auto i : tokens.back()) {
					if (isalpha(i)) {
						isLetter = true;
						break;
					}
				}
				activesheet->remove(toDelete, isLetter);
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 6) == "insert") {
			//insert esetén az input szigorúan 5 szóból kell álljon, ellenkező esetben reklamálunk
			if (words != 5) {
				std::cout << "Hibas parameterek! Az insert parancs szintaxisa: insert <darabszam> <rows/cols> <before/after> <# of row/column>" << std::endl;
			}
			else {
				int numToInsert;
				int insertInto;
				bool isLetter = false;

				tokens = tokenize(input, words, tokens);

				numToInsert = std::stoi(tokens.at(1));
				std::transform(tokens.back().begin(), tokens.back().end(), tokens.back().begin(), [](unsigned char c) { return std::toupper(c); });

				for (auto i : tokens.back()) {
					if (isalpha(i)) {
						isLetter = true;
						break;
					}
				}
				if (isLetter)
				{
					insertInto = int(tokens.back()[0]) - 64;
				}
				else {
					insertInto = std::stoi(tokens.back());
				}

				if (tokens.at(2) == "cols" && isLetter == false) {
					std::cout << "Hiba! Oszlopokat betujelek ele/moge lehet beszurni!" << std::endl;
				}
				else if (tokens.at(2) == "rows" && isLetter == true) {
					std::cout << "Hiba! Sorokat szamok ele/moge lehet beszurni!" << std::endl;
				}
				else if (isLetter && insertInto >= activesheet->getCols()) {
					std::cout << "Hiba! Nem letezo oszlop!" << std::endl;
				}
				else if (!isLetter && insertInto >= activesheet->getRows()) {
					std::cout << "Hiba! Nem letezo sor!" << std::endl;
				}
				else {
					if (tokens[tokens.size() - 2] == "after") {
						insertInto++;
					}
					activesheet->insert(numToInsert, insertInto, isLetter);
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 4) == "save")
		{
			tokens = tokenize(input, words, tokens);
			if (words == 4)
			{
				delimiter = tokens[3];
			}
			activesheet->saveToCSV(tokens[1], delimiter);

		}
		else if (input.substr(0, 4) == "swap")
		{
			int row1;
			int row2;
			int col1;
			int col2;

			tokens = tokenize(input, words, tokens);
			std::transform(tokens[1].begin(), tokens[1].end(), tokens[1].begin(), [](unsigned char c) { return std::toupper(c); });
			std::transform(tokens[2].begin(), tokens[2].end(), tokens[2].begin(), [](unsigned char c) { return std::toupper(c); });

			if (words != 3)
			{
				std::cout << "Hibas parameterezes! A swap parancs szintaxisa: swap <cella1> <cella2>" << std::endl;
			}
			else {
				row1 = std::stoi(tokens[1].substr(1, tokens[1].length() - 1));
				row2 = std::stoi(tokens[2].substr(1, tokens[2].length() - 1));

				col1 = int(tokens[1][0]) - 64;
				col2 = int(tokens[2][0]) - 64;
				if (row1 > activesheet->getRows() - 1 || row2 > activesheet->getRows() - 1)
				{
					std::cout << "Hiba! A megadott sor nagyobb mint a tablazat sorainak szama!" << std::endl;
				}
				else if (col1 > activesheet->getCols() - 1 || col2 > activesheet->getCols() - 1)
				{
					std::cout << "Hiba! A megadott oszlop nagyobb mint a tablazat oszlopainak szama!" << std::endl;
				}
				else {
					activesheet->swap(row1, row2, col1, col2);
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);

		}
		else if (input.substr(0, 5) == "align")
		{
			int row, col;
			int upperLeft_row, upperLeft_col;
			int lowerRight_row, lowerRight_col;
			bool rangeMode = false;


			tokens = tokenize(input, words, tokens);
			if (words != 3)
			{
				std::cout << "Hibas parameterezes! Az align parancs szintaxisa: align <cella> <left/right>" << std::endl;
			}
			else {
				std::transform(tokens[1].begin(), tokens[1].end(), tokens[1].begin(), [](unsigned char c) { return std::toupper(c); });
				for (auto i : tokens[1])
				{
					if (i == ':')
					{
						rangeMode = true;
						break;
					}
				}
				if (rangeMode)
				{
					std::string coord1, coord2;
					coord1 = tokens[1].substr(0, tokens[1].find(':'));
					coord2 = tokens[1].substr(tokens[1].find(':') + 1, tokens[1].length());

					upperLeft_col = std::min(coord1[0], coord2[0]) - 64;
					lowerRight_col = std::max(coord1[0], coord2[0]) - 64;

					upperLeft_row = std::min(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));
					lowerRight_row = std::max(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));

					if (upperLeft_row > activesheet->getRows() - 1 || lowerRight_row > activesheet->getRows() - 1)
					{
						std::cout << "Hiba! A megadott sor nagyobb mint a tablazat sorainak szama!" << std::endl;
					}
					else if (upperLeft_col > activesheet->getCols() - 1 || lowerRight_col > activesheet->getCols() - 1)
					{
						std::cout << "Hiba! A megadott oszlop nagyobb mint a tablazat oszlopainak szama!" << std::endl;
					}
					else {
						for (int i = upperLeft_row; i <= lowerRight_row; i++)
						{
							for (int j = upperLeft_col; j <= lowerRight_col; j++) {
								if (tokens[2] == "right") {
									activesheet->align(i, j, true);
								}
								else if (tokens[2] == "left") {
									activesheet->align(i, j, false);
								}
								else {
									std::cout << "Hiba! A cellat balra (left) vagy jobbra (right) lehet igazitani!" << std::endl;
									break;
								}
							}
						}
					}
				}
				else {
					row = std::stoi(tokens[1].substr(1, tokens[1].length() - 1));
					col = int(tokens[1][0]) - 96;
					if (row > activesheet->getRows() - 1)
					{
						std::cout << "Hiba! A megadott sor nagyobb mint a tablazat sorainak szama!" << std::endl;
					}
					else if (col > activesheet->getCols() - 1)
					{
						std::cout << "Hiba! A megadott oszlop nagyobb mint a tablazat oszlopainak szama!" << std::endl;
					}
					else {
						if (tokens[2] == "right")
						{
							activesheet->align(row, col, true);
						}
						else if (tokens[2] == "left")
						{
							activesheet->align(row, col, false);
						}
						else {
							std::cout << "Hiba! A cellat balra (left) vagy jobbra (right) lehet igazitani!" << std::endl;
						}
					}
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 7) == "sort by")
		{
			bool isColumn = false;
			bool order = true;
			int num;
			tokens = tokenize(input, words, tokens);
			if (words < 3 || words > 4)
			{
				std::cout << "Hibas parameterezes! A sort parancs szintaxisa: sort by <sor/oszlop> [opcionalisan: <asc/desc>]" << std::endl;
			}
			std::transform(tokens[2].begin(), tokens[2].end(), tokens[2].begin(), [](unsigned char c) { return std::toupper(c); });
			for (auto i : tokens[2]) {
				if (isalpha(i)) {
					isColumn = true;
					break;
				}
			}

			if (isColumn) {
				num = int(tokens[2][0]) - 64;
			}
			else {
				num = std::stoi(tokens[2]);
			}
			if (isColumn && num > activesheet->getCols() - 1)
			{
				std::cout << "Hiba! A megadott oszlop nagyobb mint a tablazat oszlopainak szama!" << std::endl;
			}
			else if (!isColumn && num > activesheet->getRows() - 1)
			{
				std::cout << "Hiba! A megadott oszlop nagyobb mint a tablazat oszlopainak szama!" << std::endl;
			}
			else {
				if (words == 4)
				{
					if (tokens[3] == "desc")
					{
						order = false;
						activesheet->sort(num, isColumn, order);
					}
					else if (tokens[3] == "asc")
					{
						order = true;
						activesheet->sort(num, isColumn, order);
					}
					else {
						std::cout << "Hiba! Rendezni novekvo (asc) vagy csokkeno (desc) sorrendben lehet!" << std::endl;
					}
				}
				else {
					activesheet->sort(num, isColumn, order);
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 5) == "clear")
		{
			tokens = tokenize(input, words, tokens);
			if (words != 2)
			{
				std::cout << "Hibas parameterezes! A clear parancs szintaxisa: clear <mettol>:<meddig>" << std::endl;
			}
			else {
				int upperLeft_row, upperLeft_col;
				int lowerRight_row, lowerRight_col;
				std::string coord1, coord2;
				std::transform(tokens[1].begin(), tokens[1].end(), tokens[1].begin(), [](unsigned char c) { return std::toupper(c); });

				coord1 = tokens[1].substr(0, tokens[1].find(':'));
				coord2 = tokens[1].substr(tokens[1].find(':') + 1, tokens[1].length());

				upperLeft_col = std::min(coord1[0], coord2[0]) - 64;
				lowerRight_col = std::max(coord1[0], coord2[0]) - 64;

				upperLeft_row = std::min(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));
				lowerRight_row = std::max(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));

				if (upperLeft_row > activesheet->getRows() - 1 || lowerRight_row > activesheet->getRows() - 1)
				{
					std::cout << "Hiba! A megadott sor nagyobb mint a tablazat sorainak szama!" << std::endl;
				}
				else if (upperLeft_col > activesheet->getCols() - 1 || lowerRight_col > activesheet->getCols() - 1)
				{
					std::cout << "Hiba! A megadott oszlop nagyobb mint a tablazat oszlopainak szama!" << std::endl;
				}
				else {
					for (int i = upperLeft_row; i <= lowerRight_row; i++) {
						for (int j = upperLeft_col; j <= lowerRight_col; j++) {
							activesheet->clear(i, j);
						}
					}
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 8) == "barchart")
		{
			tokens = tokenize(input, words, tokens);
			if (words != 3)
			{
				std::cout << "Hibas parameterezes! A barchart parancs szintaxisa: barchart <MN:XY> <fajlnev>" << std::endl;
			}
			else {
				int upperLeft_row, upperLeft_col;
				int lowerRight_row, lowerRight_col;
				std::string coord1, coord2;
				std::transform(tokens[1].begin(), tokens[1].end(), tokens[1].begin(), [](unsigned char c) { return std::toupper(c); });

				coord1 = tokens[1].substr(0, tokens[1].find(':'));
				coord2 = tokens[1].substr(tokens[1].find(':') + 1, tokens[1].length());

				upperLeft_col = std::min(coord1[0], coord2[0]) - 64;
				lowerRight_col = std::max(coord1[0], coord2[0]) - 64;

				upperLeft_row = std::min(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));
				lowerRight_row = std::max(std::stoi(coord1.substr(1, coord1.length())), std::stoi(coord2.substr(1, coord2.length())));
				if (upperLeft_col == lowerRight_col || upperLeft_row == lowerRight_row)
				{
					std::cout << "Hiba! Nem megfelelo tartomany!" << std::endl;
				}
				else if (upperLeft_row > activesheet->getRows() - 1 || lowerRight_row > activesheet->getRows() - 1)
				{
					std::cout << "Hiba! A megadott sor nagyobb mint a tablazat sorainak szama!" << std::endl;
				}
				else if (upperLeft_col > activesheet->getCols() - 1 || lowerRight_col > activesheet->getCols() - 1)
				{
					std::cout << "Hiba! A megadott oszlop nagyobb mint a tablazat oszlopainak szama!" << std::endl;
				}
				else {
					activesheet->renderChart(upperLeft_row, upperLeft_col, lowerRight_row, lowerRight_col, tokens[2]);
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 9) == "new sheet")
		{
			tokens = tokenize(input, words, tokens);
			if (words < 3)
			{
				std::cout << "Hibas parameterezes! A new sheet parancs szintaxisa: new sheet <nev>" << std::endl;
			}
			else {
				sheets.push_back(new Spreadsheet(1, 1, tokens[2]));
				sheets.back()->setID(sheets.size() - 1);
				activesheet = sheets.back();
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}

		else if (input.substr(0, 6) == "switch")
		{
			if (words != 2)
			{
				std::cout << "Hibas parameterezes! A switch parancs szintaxisa: switch <index>" << std::endl;
			}
			else {
				tokens = tokenize(input, words, tokens);
				try
				{
					if (std::stoi(tokens[1]) > sheets.size() - 1)
					{
						std::cout << "Hiba! Nincs ilyen indexu tablazat!" << std::endl;
					}
					else if (std::stoi(tokens[1]) < 0)
					{
						std::cout << "Hiba! Nincs ilyen indexu tablazat!" << std::endl;
					}
					else {
						activesheet = sheets[std::stoi(tokens[1])];
					}
				}
				catch (const std::exception&)
				{
					std::cout << "Hiba! Szamot adj meg!" << std::endl;
				}
				
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}

		else if (input.substr(0, 4) == "open")
		{
			if (words != 2 && words != 4)
			{
				std::cout << "Hibas parameterezes! Az open parancs szintaxisa: open <fajlnev> [opcionalisan: -sep <karakter>]" << std::endl;
			}
			else {
				tokens = tokenize(input, words, tokens);
				maxLength = 0;
				file = tokens[1];
				if (words == 4)
				{
					delimiter = tokens[3];
				}
				csvToVector = readCSV(file, delimiter);
				for (auto i : csvToVector)
				{
					if (i.size() > maxLength)
					{
						maxLength = i.size();
					}
				}
				sheets.push_back(new Spreadsheet(csvToVector.size(), maxLength, file.substr(0, file.find(".csv"))));
				sheets.back()->fillVectorFromFile(csvToVector);
				sheets.back()->setID(sheets.size() - 1);
				activesheet = sheets.back();
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input.substr(0, 5) == "close")
		{
			if (words != 2)
			{
				std::cout << "Hibas parameterezes! A close parancs szintaxisa: close <index>" << std::endl;
			}
			else {
				tokens = tokenize(input, words, tokens);
				try
				{
					if (std::stoi(tokens[1]) > sheets.size() - 1)
					{
						std::cout << "Hiba! Nincs ilyen indexu tablazat!" << std::endl;
					}
					else if (std::stoi(tokens[1]) < 0)
					{
						std::cout << "Hiba! Nincs ilyen indexu tablazat!" << std::endl;
					}
					else {
						if (sheets.size() > 1)
						{
							for (int i = 0; i < sheets[std::stoi(tokens[1])]->getVector().size(); i++)
							{
								for (int j = 0; j < sheets[std::stoi(tokens[1])]->getVector()[i].size(); j++)
								{
									delete sheets[std::stoi(tokens[1])]->getVector()[i][j];
								}
							}
							sheets[std::stoi(tokens[1])]->getVector().clear();
							delete sheets[std::stoi(tokens[1])];
							sheets.erase(sheets.begin() + std::stoi(tokens[1]));
							if (activesheet->getID() == std::stoi(tokens[1]))
							{
								if (std::stoi(tokens[1]) == 0)
								{
									activesheet = sheets[std::stoi(tokens[1]) + 1];
								}
								else {
									activesheet = sheets[std::stoi(tokens[1]) - 1];
								}
							}
							for (int i = 0; i < sheets.size(); i++)
							{
								sheets[i]->setID(i);
							}
						}
						else {
							for (auto sheet : sheets)
							{
								for (int i = 0; i < sheet->getVector().size(); i++)
								{
									for (int j = 0; j < sheet->getVector()[i].size(); j++)
									{
										delete sheet->getVector()[i][j];
									}
								}
								sheet->getVector().clear();
								delete sheet;
							}
							return 0;
						}
					}
				}
				catch (const std::exception&)
				{
					std::cout << "Hiba! Szamot adj meg!" << std::endl;
				}
				
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}

		else if (input.substr(0, 6) == "rename")
		{
			if (words < 3)
			{
				std::cout << "Hibas parameterezes! A rename parancs szintaxisa: rename <index> <nev>" << std::endl;
			}
			else {
				tokens = tokenize(input, words, tokens);
				try
				{
					if (std::stoi(tokens[1]) > sheets.size() - 1)
					{
						std::cout << "Hiba! Nincs ilyen indexu tablazat!" << std::endl;
					}
					else if (std::stoi(tokens[1]) < 0)
					{
						std::cout << "Hiba! Nincs ilyen indexu tablazat!" << std::endl;
					}
					else {
						sheets[std::stoi(tokens[1])]->setName(tokens[2]);
					}
				}
				catch (const std::exception&)
				{
					std::cout << "Hiba! Szamot adj meg!" << std::endl;
				}
			}
			activesheet->print();
			printSheets(sheets, activesheet);
		}
		else if (input == "exit")
		{
			for (auto sheet : sheets)
			{
				for (int i = 0; i < sheet->getVector().size(); i++)
				{
					for (int j = 0; j < sheet->getVector()[i].size(); j++)
					{
						delete sheet->getVector()[i][j];
					}
				}
				sheet->getVector().clear();
				delete sheet;
			}
			return 0;
		}
		else if (input != "exit") {
			std::cout << "Ismeretlen parancs!" << std::endl;
			activesheet->print();
			printSheets(sheets, activesheet);
		}
	}
}

std::vector<std::string> tokenize(std::string input, int words, std::vector<std::string> tokens) {
	tokens.erase(tokens.begin(), tokens.end());
	if (input.substr(0, 4) == "edit" || input.substr(0, 9) == "new sheet" || input.substr(0, 6) == "rename")
	{
		for (int i = 0; i < 2; i++) {
			tokens.push_back(input.substr(0, input.find_first_of(" ")));
			input.erase(0, input.find_first_of(" ") + 1);
		}
		tokens.push_back(input);
		input.erase(input.begin(), input.end());
	}
	else {
		for (int i = 0; i < words; i++) {
			tokens.push_back(input.substr(0, input.find_first_of(" ")));
			input.erase(0, input.find_first_of(" ") + 1);
		}
	}
	return tokens;
}

std::vector<std::vector<std::string>> readCSV(std::string file, std::string delimiter) {
	std::ifstream csvfile(file);
	std::string row;
	std::vector<std::vector<std::string> > parsedCsv;
	while (std::getline(csvfile, row))
	{
		std::stringstream ss(row);
		std::string cell;
		std::vector<std::string> parsedRow;
		while (std::getline(ss, cell, delimiter[0]))
		{
			if (int(cell[cell.size() - 1]) == 13)
			{
				cell.pop_back();
			}
			parsedRow.push_back(cell);
		}
		parsedCsv.push_back(parsedRow);
	}
	csvfile.close();
	return parsedCsv;
}
bool checkCLIArguments(int argc, char** argv) {
	if (argc > 2)
	{
		if (argc > 2 && argc != 4)
		{
			std::cout << "Hibas parameterek! A helyes szintaxis: ./a.out <fajlnev> [opcionálisan: <-sep> <elvalaszto kar.>]" << std::endl;
			return 1;
		}
		else if (std::string(argv[2]) != "-sep")
		{
			std::cout << "Ismeretlen kapcsolo! Az alkalmazhato kapcsolok listaja:\n-sep" << std::endl;
			return 1;
		}
		else if (std::string(argv[3]) == ";")
		{
			std::cout << "Hiba! A ';' az alapertelmezett elvalaszto, nem allithato be!" << std::endl;
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
int getInputWordCount(std::string input) {
	size_t words = input.empty() || input.back() == ' ' ? 0 : 1;
	for (size_t s = input.size(); s > 0; s--) {
		if (input[s] == ' ' && input[s - 1] != ' ') {
			words++;
		}
	}
	return words;
}

void printSheets(std::vector<Spreadsheet*> sheets, Spreadsheet* activesheet) {
	for (int i = 0; i < sheets.size(); i++)
	{
		std::cout << sheets[i]->getID();
		if (sheets[i]->getID() == activesheet->getID())
		{
			std::cout << "*";
		}
		std::cout << ": " << sheets[i]->getName();
		std::cout << "\t";
	}
	std::cout << "\n\n";
}