/**
 * \class Spreadsheet
 *
 * \brief Description of the Spreadsheet class
 *
 * The spreadsheet class handles the collection of cells.
*/

#ifndef SPREADSHEET_HEADER
#define SPREADSHEET_HEADER
#include <vector>
#include <string>
#include "Cell.h"

class Spreadsheet {
protected:
	int rows;                               ///<The number of rows in the spreadsheet
	int cols;                               ///<The number of columns in the spreadsheet
	std::vector<std::vector<Cell*>> vect;   ///<The spreadsheet itself is stored in a two-dimensional vector of Cell class objects
	std::vector<Cell> functions;            ///<The cells that contain functions are stored in a separate vector as well 
	std::string name;						///<The name of the table
	int id;									///<The id of the table (incrementing from 0)
public:

	/**
	 * \brief The constructor of the spreadsheet, constructs an empty spreadsheet with given rows and columns.
	 * \param rows the number of rows
	 * \param cols the number of columns
	 * \param name the name of the table, by default "Table"
	*/
	Spreadsheet(int rows, int cols, std::string name = "Table");

	/// \brief The destructor of the spreadsheet
	~Spreadsheet();

	/// \brief The print function prints out the current state of the table after each command
	void print();

	/// \brief Returns the number of rows
	int getRows();

	/// \brief Returns the number of columns
	int getCols();

	/// \brief Returns the length of the longest element in the column as the column's width
	/// \param col the # of the column to obtain the width from
	int getColWidth(int col);

	/// \brief Sets the number of rows
	/// \param newrows the new number of rows
	void setRows(int newrows);

	/// \brief Sets the number of columns
	/// \param newcols the new number of columns
	void setCols(int newcols);

	/// \brief Creates the shell of the spreadsheet: labels the columns and rows
	/// \param rows the number of rows to create
	/// \param cols the number of columns to create
	void createVectorSkeleton(int rows, int cols);

	/// \brief Remakes the shell of the spreadsheet, i.e. after inserting new rows/columns the labeling gets ruined, this function corrigates it.
	void fixVectorSkeleton();

	/// \brief Edits a certain cell of the spreadsheet
	/// \param row the row
	/// \param col the column
	/// \param value the value that's inserted into the cell
	void edit(int row, int col, std::string value);

	/// \brief Adds N empty rows or columns to the end of the spreadsheet
	/// \param num the number of rows/columns to add
	/// \param dimension if 0, the function adds columns to the spreadsheet, else it adds rows
	void add(int num, bool dimension);

	/// \brief Deletes an entire row/column
	/// \param toDelete the number/letter of the row/column that's to be deleted
	/// \param isColumn if false, the function deletes a row, else it deletes a column
	void remove(std::string toDelete, bool isColumn);

	/// \brief Inserts N empty rows/columns to a certain position
	/// \param numToInsert the number of rows/columns to be inserted
	/// \param insertInto the position of the insertion
	/// \param isColumn if false, the function inserts rows, else it inserts columns
	void insert(int numToInsert, int insertInto, bool isColumn);

	/// \brief Swaps the values of two cells
	/// \param row1 the row of the first cell
	/// \param col1 the column of the first cell
	/// \param row2 the row of the other cell
	/// \param col2 the column of the other cell
	void swap(int row1, int col1, int row2, int col2);

	/**
	 * \brief Sorts the contents of a row/column either in ascending or descending order
	 *
	 * More specifically it sorts the numeric and alphabetic values separately\n
	 * Then the order of sorting is as follows: strings < numbers
	 * \param num the number of row/column to sort
	 * \param isColumn if true: it sorts a column, else it sorts a row
	 * \param order if true (by default): it sorts in ascending order, else it sorts in descending order
	*/
	void sort(int num, bool isColumn, bool order);

	/// \brief Aligns the content of a cell left or right
	/// \param row the row of the cell
	/// \param col the column of the cell
	/// \param alignment if true: it aligns the cell right, else it aligns left
	void align(int row, int col, bool alignment);

	/// \brief Sets the value of a cell in the spreadsheet to an empty string (works only with a given range of the spreadsheet)
	/// \param row the row of the cell to be cleared
	/// \param col the column of the cell to be cleared
	void clear(int row, int col);

	/// \brief Fills the cells of the spreadsheet with the contents a .csv file that has been previously read into a vector
	/// \param csv a two-dimensional vector of strings that contains the values of the .csv file
	void fillVectorFromFile(std::vector<std::vector<std::string>> _CrtIsValidHeapPointer);

	/// \brief Writes the contents of the spreadsheet to a .csv file
	/// \param filename the name of the .csv file
	/// \param delimiter the delimiter to separate the values with (by default: ';')
	void saveToCSV(std::string filename, std::string deleimiter);

	/// \brief Checks if the coordinates passed as a function's parameters are within boundaries of the spreadsheet, if not, it returns false
	/// \param coords The coordinates passed to the function as parameters
	bool functionInputCheck(std::string coords);

	/// \brief Sets each cell's coordinates to its current row and column
	void setCellCoords();

	/// \brief Evaluates every function in the table
	void evaluateFunctions();

	/// \brief Checks for circular links throughout the functions of the table, if it finds any, then it sets those cell's value to "#CYCLE?"
	/// \param functions the vector containing every function in the table\n
	/// The function starts with a for loop through every function, this variable is called **i**\n
	/// Then it loops through the given function's linked cells, this variable is called **cell**\n
	/// Another for loop through the vector of functions, this variable is called **j**\n 
	/// If **cell.coordinates == j.coordinates** then another for loop through **j's** linked cells, this variable is called **cell2**\n
	/// If **cell2.coordinates == i.coordinates** then the two cells are linked circularly, both **i** and **j** are set to "#CYCLE?"\n
	/// It also works if a cell is linked to itself, in which case only that cell's value gets set to "#CYCLE?"
	void checkCircularFunctions(std::vector<Cell> functions);

	/// \brief loops through the table, and every cell with its **functionMode** parameter set to true gets put into a vector called 'functions'
	void findFunctions();

	/// \brief Returns the complete vector 
	/// This function only serves testing purposes
	std::vector<std::vector<Cell*>> getVector();

	/// \brief Sets every cell's displayed value, based on the state of the **functionMode** and **errorInFunction** booleans
	void setDisplayedValues();

	/// \brief SVG renderer
	/// \param upperleft_row The row of the upper-left coordinate
	/// \param upperleft_col The col of the upper-left coordinate
	/// \param lowerright_row The row of the lower-right coordinate
	/// \param lowerright_col The col of the lower-right coordinate
	/// \param file the filename we save the SVG as\n
	/// **IMPORTANT** this piece of code is based on this solution, all credits to Matt Grover: https://codepen.io/mdgrover/pen/eZENxO
	void renderChart(int upperleft_row, int upperleft_col, int lowerright_row, int lowerright_col, std::string file);
	
	/// \brief Returns the name of the table
	std::string getName();

	/// \brief Sets the name of the table
	/// \param name_ the new name of the table
	void setName(std::string name_);

	/// \brief Returns the ID of the table
	int getID();

	/// \brief Sets the ID of the table
	/// \param id_ the new ID of the table
	void setID(int id_);
};

#endif