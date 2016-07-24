
/**
@file
Include file for LiquidMenu library.

@author Vasil Kalchev

*/

#pragma once

#include <LiquidCrystal.h>

#include "LiquidMenu_config.h"

#ifndef LiquidCrystal_h
#warning "LiquidCrystal library is required!"
#endif

const char VERSION[] = "1.0"; ///< The version of the library.
const uint8_t MAX_VARIABLES = 4; ///< Sets the size of the `void*` array.
const uint8_t DIVISION_LINE_LENGTH = 40; ///< Sets the length of the division line.
const char NOTHING[] = "                    "; ///< 20 spaces, used to clear the display.

/// Data type enum.
/**
Used to store the data type of `void*` so that they can be cast back later.
*/
enum class DataType : uint8_t {
	NOT_USED = 0,
	BOOL = 1, BOOLEAN = 1,
	INT8_T = 8,
	UINT8_T = 9, BYTE = 9,
	INT16_T = 16,
	UINT16_T = 17,
	INT32_T = 32,
	FLOAT = 50, DOUBLE = 50,
	CHAR = 60,
	CHAR_PTR = 61,
	CONST_CHAR_PTR = 62,
};

/// Position enum.
/*
Used to store and set the relative or absolute position of the focus indicator.
*/
enum class Position : uint8_t {
	RIGHT = 1, NORMAL = 1,
	LEFT = 2,
	CUSTOM = 3,
};

/// @name recognizeType overloaded function
/**
Used to recognize the data type of a variable received in a template function.
@see DataType
*/
/**@{*/

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(bool variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(char variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(char* variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(const char* variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(int8_t variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(uint8_t variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(int16_t variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(uint16_t variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(int32_t variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(uint32_t variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(float variable);

/**@}*/


/// Prints the number passed to it in a specific way.
/**
Used for convenience when printing the class' address for indentification.

@param address - number to be printed
*/
void print_me(uint16_t address);


/// Represents the individual lines printed on the display.
/**
This is the lowest class in the hierarchy, it holds pointers to the
variables/constants that will be printed, where the line is positioned,
where the focus indicator is positioned and pointers to the callback
functions. This classes' objects go into a LiquidScreen object which
which controls them. The public methods are for configuration only.
*/
class LiquidLine {
	friend class LiquidScreen;

public:
	/// @name Constructors
	/**@{*/

	/// Constructor for one variable/constant.
	/**
	This is the main constructor that gets called every time.

	@param column - the column at which the line starts
	@param row - the row at which the line is printed
	@param &variableA - variable/constant to be printed
	*/
	template <typename A>
	LiquidLine(uint8_t column, uint8_t row, A &variableA)
		: _row(row), _column(column), focusRow(row - 1),
		  _fcousColumn(column - 1), _focusPosition(Position::NORMAL),
		  _focusable(false) {
		for (uint8_t i = 0; i < MAX_VARIABLES; i++) {
			_variable[i] = nullptr;
			_variableType[i] = DataType::NOT_USED;
		}
		for (uint8_t f = 0; f < MAX_FUNCTIONS; f++) {
			_function[f] = 0;
		}
		_variable[0] = (void*)&variableA;
		_variableType[0] = recognizeType(variableA);
	}

	/// Constructor for two variables/constants.
	/**
	@param column - the column at which the line starts
	@param row - the row at which the line is printed
	@param &variableA - variable/constant to be printed
	@param &variableB - variable/constant to be printed
	*/
	template <typename A, typename B>
	LiquidLine(uint8_t column, uint8_t row,
	           A &variableA, B &variableB)
		: LiquidLine(column, row, variableA) {
		_variable[1] = (void*)&variableB;
		_variableType[1] = recognizeType(variableB);
	}

	/// Constructor for three variables/constants.
	/**
	@param column - the column at which the line starts
	@param row - the row at which the line is printed
	@param &variableA - variable/constant to be printed
	@param &variableB - variable/constant to be printed
	@param &variableC - variable/constant to be printed
	*/
	template <typename A, typename B, typename C>
	LiquidLine(uint8_t column, uint8_t row,
	           A &variableA, B &variableB, C &variableC)
		: LiquidLine(column, row, variableA, variableB) {
		_variable[2] = (void*)&variableC;
		_variableType[2] = recognizeType(variableC);
	}

	/// Constructor for three variables/constants.
	/**
	@param column - the column at which the line starts
	@param row - the row at which the line is printed
	@param &variableA - variable/constant to be printed
	@param &variableB - variable/constant to be printed
	@param &variableC - variable/constant to be printed
	*/
	template <typename A, typename B, typename C, typename D>
	LiquidLine(uint8_t column, uint8_t row,
	           A &variableA, B &variableB, C &variableC, D &variableD)
		: LiquidLine(column, row, variableA, variableB, variableC) {
		_variable[3] = (void*)&variableD;
		_variableType[3] = recognizeType(variableD);
	}

	/**@}*/


	/// @name Public methods
	/**@{*/

	/// Attaches a callback function to the line.
	/**
	Attaches a callback function to this line. The number is used for
	identification. The callback function can later be called when the
	line is focused with `LiquidMenu::call_function(uint8_t number)`.

	@param number - function number used for identification
	@param *function - pointer to the function
	@returns true on success and false if maximum amount of functions
	has been reached

	@note Function numbering starts from 1.
	@note The maximum amount of functions per line is specified in
	LiquidMenu_config.h as `MAX_FUNCTIONS`. The default is 8.

	@see LiquidMenu_config.h
	@see MAX_FUNCTIONS
	@see LiquidMenu::call_function(uint8_t number)
	*/
	bool attach_function(uint8_t number, void (*function)(void));

	/// Configures the focus indicator position for the line.
	/**
	The valid positions are `LEFT`, `RIGHT` and `CUSTOM`. The `CUSTOM`
	position is absolute so it also needs the column and row that it
	will be printed on.

	@param position - `LEFT`, `RIGHT` or `CUSTOM`
	@param column - if using `CUSTOM` this specifies the column
	@param row - if using `CUSTOM` this specifies the row
	@returns true on success and false if the position specified is
	invalid

	@note The `Position` is enum class. Use `Position::(member)` when
	specifeing the position.

	@see Position
	*/
	bool set_focusPosition(Position position,
	                       uint8_t column = 0, uint8_t row = 0);

	/**@}*/

private:
	/// Prints the line to the display.
	/**
	Sets the cursor to the starting position. Then goes through a loop
	calling `print_variable(LiquidCrystal *p_liquidCrystal, uint8_t number)`.
	And finally displays the focus indicator if the line is focused.

	@param *p_liquidCrystal - pointer to the LiquidCrystal object
	@param isFocused - true if this line is focused
	*/
	void print(LiquidCrystal *p_liquidCrystal, bool isFocused);

	/// Prints a variable to the display.
	/**
	Casts the variable pointer specified by the number to its data type
	and prints it to the display.

	@param *p_liquidCrystal - pointer to the LiquidCrystal object
	@param number - number identifeing the variable
	*/
	void print_variable(LiquidCrystal *p_liquidCrystal, uint8_t number);

	/// Calls an attached function specified by the number.
	/**
	@param number - number identifeing the function
	@returns true if there is a function at the specified number

	@note Function numbering starts from 1.

	@see bool LiquidLine::attach_function(uint8_t number, void (*function)(void));
	*/
	bool call_function(uint8_t number) const;

	uint8_t _row, _column, _focusRow, _focusColumn;
	Position _focusPosition;
	void (*_function[MAX_FUNCTIONS])(void); ///< Pointers to the functions
	const void *_variable[MAX_VARIABLES]; ///< Pointers to the variables
	DataType _variableType[MAX_VARIABLES]; ///< Data type of the variables
	bool _focusable; ///< Determines wheter the line is focusable
};

