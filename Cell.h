/**
 * \class Cell
 * 
 * \brief Description of the Cell class
 * 
 * A Cell class contains each property of a cell:
 * -its regular value, that the user enters
 * -if that value is a function, it stores the evaluated value
 * -if the function ends up in an error, it stores the error message
 * -one of these three is going to be the actual value to be displayed
 * 
 * -it contains the cells that are linked to it by a function
 * -lastly it contains the cell's coordinates: its row, and its column
*/

#ifndef CELL_HEADER
#define CELL_HEADER

#include <string>
#include <vector>

class Cell {
protected:
    std::string value;              ///< the value of the cell
    bool alignment;                 ///< the alignment of the cell;
    bool functionMode;              ///< switch between displaying the standard or evaluated value of the cell
	double functionValue;           ///< the result of the function, if it doesn't end up in an error
	int row;                        ///< the row where the cell is located
	int col;                        ///< the column where the cell is located
	bool errorInFunction;           ///< true if the function evaluates to an error e.g. circular functions
	std::string errValue;           ///< the error message: either "#NAME?" or "#CYCLE?"
	std::vector<Cell> linkedCells;  ///< a vector of each cell linked to the current cell by its function.
	std::string displayedValue;     ///< the actual value to display when printing the output
public:
    /**
     * \brief Constructor of the cell class
     * \param value the value of the cell
     * \param alignment the alignment of the cell
     * \param functionMode switch between displaying the regular and evaluated value of the cell
     * \param functionValue the value evaluated by the function
    */
    Cell(std::string value, bool alignment, bool functionMode, double functionValue);

    /**
     * \brief The copy constructor of the Cell class
     * 
     * To my best knowledge, a copy constructor is neccessary because as we populate our spreadsheet vector\n 
     * with multiple cells, the original constructor is only called at the 0th element of the vector, for the rest\n
     * it automatically tries to use a copy constructor
     * \param cell_ the original Cell to copy
    */
    Cell(const Cell&);

    /**
     * The destructor of the Cell class
    */
    ~Cell();

    /// \brief Returns the row of the cell
    int getRow();

    /// \brief Returns the column of the cell
	int getCol();

    /// \brief Sets the row of the cell
    /// \param row the number to set as the row of the cell
	void setRow(int row);

    /// \brief Sets the column of the cell
    /// \param col the number to set as the column of the cell
	void setCol(int col);

    /// \brief Returns the value displayed in the cell
	std::string getDisplayedValue();

    /// \brief Sets the value to display in the cell
    /// \param value the value to display
	void setDisplayedValue(std::string value);
	
    /// \brief Returns true, if the value emitted is a function, whether it ends up in an error or not
	bool getFunctionMode();

    /// \brief Sets the switch between displaying the regular, or the evaluated value of the cell
    /// \param fMode true if the value emitted is a function, else its false
	void setFunctionMode(bool fMode);

    /// \brief Returns the value evaluated by the function
    double getFunctionValue();

    /// \brief Sets the value evaluated by the function
    /// \param result the result of the function
	void setFunctionValue(double result);

    /// \brief Returns the error message, if the function ends up in one. The value is either "#NAME?" or "#CYCLE?"
	std::string getErrValue();

    /// \brief Sets the error message, if the function ends up in one.
    /// \param value the error value, which is either "#NAME?" or "#CYCLE?"
	void setErrValue(std::string value);

    /// \brief Returns true if the function ends up in an error
    bool getErrorInFunction();

    /// \brief Sets the switch between displaying the result of the function (false), or the error message (true), if the function ends up in one.
    /// \param errMode true if the function yields an error, else its false
	void setErrorInFunction(bool errMode);

    /// \brief Links each cell of the function's parameter to the cell where the function was entered
    /// \param cell the cell to link
	void setLinkedCell(Cell cell);

    /// \brief Returns each cell that are linked to the current cell
	std::vector<Cell> getLinkedCells();

    /// \brief Removes all the links from the current cell
	void clearLinkedCells();

    /// \brief Returns the entered value of the cell
    std::string getValue();

    /// \brief Returns the alignment of the cell
    bool getAlignment();

    /// \brief Sets the entered value of the cell
    /// \param value the entered value of the cell
    void setValue(std::string value);

    /// \brief Sets the alignment of the cell
    /// \param alignment the new alignment of the cell
    void setAlignment(bool alignment);

};

#endif