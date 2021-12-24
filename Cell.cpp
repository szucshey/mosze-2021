#include "Cell.h"
#include <iostream>

Cell::Cell(std::string value, bool alignment, bool functionMode, double functionValue) : value(value), alignment(alignment), functionMode(functionMode), functionValue(functionValue) {}

Cell::Cell(const Cell &cell_){ 
    value = cell_.value;
	alignment = cell_.alignment;
	functionValue = 0.00;
	functionMode = false;
	errorInFunction = false;
	errValue = "";
	row = cell_.row;
	col = cell_.col;
	linkedCells = cell_.linkedCells;
	displayedValue = cell_.displayedValue;
}

Cell::~Cell() {}

int Cell::getRow() {
	return row;
}

int Cell::getCol() {
	return col;
}

void Cell::setRow(int row_) {
	row = row_;
}

void Cell::setCol(int col_) {
	col = col_;
}

std::string Cell::getDisplayedValue()
{
	return displayedValue;
}

void Cell::setDisplayedValue(std::string dValue)
{
	displayedValue = dValue;
}

std::string Cell::getValue() {
	return value;
}

bool Cell::getAlignment() {
	return alignment;
}

double Cell::getFunctionValue() {
	return functionValue;
}

bool Cell::getFunctionMode() {
	return functionMode;
}

void Cell::setValue(std::string value_) {
	value = value_;
}

void Cell::setAlignment(bool alignment_) {
	alignment = alignment_;
}

void Cell::setFunctionValue(double fValue_) {
	functionValue = fValue_;
}

void Cell::setFunctionMode(bool fMode_) {
	functionMode = fMode_;
}

std::string Cell::getErrValue()
{
	return errValue;
}

void Cell::setErrValue(std::string errValue_)
{
	errValue = errValue_;
}

void Cell::setLinkedCell(Cell cell_) {
	linkedCells.push_back(cell_);
}

std::vector<Cell> Cell::getLinkedCells() {
	return linkedCells;
}

void Cell::clearLinkedCells() {
	linkedCells.clear();
}

bool Cell::getErrorInFunction() {
	return errorInFunction;
}
void Cell::setErrorInFunction(bool err) {
	errorInFunction = err;
}

