/*
The MIT License (MIT)

Copyright (c) 2016 Vasil Kalchev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
@file
Include file for LiquidMenu library.

@author Vasil Kalchev
@date 2016
@version 1.5.0
@copyright The MIT License

@todo: Change/Remove variables/screens/menus maybe
@todo: screen wide glyphs
@todo: dynamic memory
@todo: variadic templates
*/

#pragma once

#include <stdint.h>
#if defined(__AVR__)
# include <avr/pgmspace.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "LiquidMenu_config.h"
#include "LiquidMenu_debug.h"


#if LIQUIDMENU_LIBRARY == LiquidCrystal_LIBRARY
# pragma message ("LiquidMenu: Selected 'LiquidCrystal' (parallel) library. Edit 'LiquidMenu_config.h' file to change it.")
#elif LIQUIDMENU_LIBRARY == LiquidCrystal_I2C_LIBRARY
# pragma message ("LiquidMenu: Selected 'LiquidCrystal_I2C' (I2C) library. Edit 'LiquidMenu_config.h' file to change it.")
#else
# pragma message ("LiquidMenu: Selected custom library. Edit 'LiquidMenu_config.h' file to change it.")
#endif

#if LIQUIDMENU_DEBUG
# warning "LiquidMenu: Debugging messages are enabled."
#endif


typedef bool (*boolFnPtr)();
typedef int8_t (*int8tFnPtr)();
typedef uint8_t (*uint8tFnPtr)();
typedef int16_t (*int16tFnPtr)();
typedef uint16_t (*uint16tFnPtr)();
typedef int32_t (*int32tFnPtr)();
typedef uint32_t (*uint32tFnPtr)();
typedef float (*floatFnPtr)();
typedef double (*doubleFnPtr)();
typedef char (*charFnPtr)();
typedef char * (*charPtrFnPtr)();
typedef const char * (*constcharPtrFnPtr)();

const char LIQUIDMENU_VERSION[] = "1.5"; ///< The version of the library.

/// Data type enum.
/**
Used to store the data type of `void*` so that they can be cast back later.
*/
enum class DataType : uint8_t {
  NOT_USED = 0,
  BOOL = 1, BOOLEAN = 1,
  INT8_T = 8,
  UINT8_T = 9,
  INT16_T = 16,
  UINT16_T = 17,
  INT32_T = 32,
  UINT32_T = 33,
  FLOAT = 50, DOUBLE = 50,
  CHAR = 60,
  CHAR_PTR = 61,
  CONST_CHAR_PTR = 62,
  PROG_CONST_CHAR_PTR = 65,
  GLYPH = 70,
  BOOL_GETTER = 201, BOOLEAN_GETTER = 201,
  INT8_T_GETTER = 208,
  UINT8_T_GETTER = 209, BYTE_GETTER = 209,
  INT16_T_GETTER = 216,
  UINT16_T_GETTER = 217,
  INT32_T_GETTER = 232,
  UINT32_T_GETTER = 233,
  FLOAT_GETTER = 240, DOUBLE_GETTER = 240,
  CHAR_GETTER = 250,
  CHAR_PTR_GETTER = 251,
  CONST_CHAR_PTR_GETTER = 252
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
///@{

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

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(double variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(boolFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(int8tFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(uint8tFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(int16tFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(uint16tFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(int32tFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(uint32tFnPtr varible);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(floatFnPtr variable);

/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(doubleFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(charFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(charPtrFnPtr variable);


/**
@param variable - variable to be checked
@returns the data type in `DataType` enum format
*/
DataType recognizeType(constcharPtrFnPtr variable);
///@}



/// Prints the number passed to it in a specific way.
/**
Used for convenience when printing the class's address for indentification.

@param address - number to be printed
*/
void print_me(uintptr_t address);


/// Represents the individual lines printed on the display.
/**
This is the lowest class in the hierarchy, it holds pointers to the
variables/constants that will be printed, where the line is positioned,
where the focus indicator is positioned and pointers to the callback
functions. This classes' objects go into a LiquidScreen object which
controls them. The public methods are for configuration only.
*/
class LiquidLine {
  friend class LiquidScreen;

public:
  /// @name Constructors
  ///@{

  /// The main constructor.
  /**
  This is the main constructor that gets called every time.

  @param column - the column at which the line starts
  @param row - the row at which the line is printed
  */
  LiquidLine(uint8_t column, uint8_t row)
    : _row(row), _column(column), _focusRow(row - 1),
      _focusColumn(column - 1), _focusPosition(Position::NORMAL),
      _variableCount(0), _focusable(false) {
    for (uint8_t i = 0; i < MAX_VARIABLES; i++) {
      _variable[i] = nullptr;
      _variableType[i] = DataType::NOT_USED;
    }
    for (uint8_t f = 0; f < MAX_FUNCTIONS; f++) {
      _function[f] = 0;
    }
	  _floatDecimalPlaces = 2;
  }

  /// Constructor for one variable/constant.
  /**
  @param column - the column at which the line starts
  @param row - the row at which the line is printed
  @param &variableA - variable/constant to be printed
  */
  template <typename A>
  LiquidLine(uint8_t column, uint8_t row, A &variableA)
    : LiquidLine(column, row) {
    add_variable(variableA);
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
    add_variable(variableB);
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
    add_variable(variableC);
  }

  /// Constructor for four variables/constants.
  /**
  @param column - the column at which the line starts
  @param row - the row at which the line is printed
  @param &variableA - variable/constant to be printed
  @param &variableB - variable/constant to be printed
  @param &variableC - variable/constant to be printed
  @param &variableD - variable/constant to be printed
  */
  template <typename A, typename B, typename C, typename D>
  LiquidLine(uint8_t column, uint8_t row,
             A &variableA, B &variableB, C &variableC, D &variableD)
    : LiquidLine(column, row, variableA, variableB, variableC) {
    add_variable(variableD);
  }

  ///@}


  /// @name Public methods
  ///@{

  /// Adds a variable to the line.
  /**
  @param &variable - reference to the variable
  @returns true on success and false if the maximum amount of variables
  has been reached

  @note The maximum amount of variable per line is specified in
  LiquidMenu_config.h as `MAX_VARIABLES`. The default is 5.

  @see LiquidMenu_config.h
  @see MAX_VARIABLES
  */
  template <typename T>
  bool add_variable(T &variable) {
    print_me(reinterpret_cast<uintptr_t>(this));
    if (_variableCount < MAX_VARIABLES) {
      _variable[_variableCount] = (void*)&variable;
      _variableType[_variableCount] = recognizeType(variable);
#     if LIQUIDMENU_DEBUG
        DEBUG(F("Added variable "));
        // Check if the variable is actually a getter functions
        // and don't diplay it if so.
        if ((uint8_t)_variableType[_variableCount] < 200) { // 200+ are getters
          DEBUG(reinterpret_cast<uintptr_t>(variable)); DEBUGLN(F(""));
        }
#     endif
      _variableCount++;
      return true;
    }
#   if LIQUIDMENU_DEBUG
      DEBUG(F("Adding variable "));
      // Check if the variable is actually a getter functions
      // and don't diplay it if so.
      if ((uint8_t)_variableType[_variableCount] < 200) { // 200+ are getters
        DEBUG(reinterpret_cast<uintptr_t>(variable));
      }
#   endif
    DEBUGLN(F(" failed, edit LiquidMenu_config.h to allow for more variables"));
    return false;
  }

  /// Attaches a callback function to the line.
  /**
  The number is used for identification. The callback function can
  later be called when the line is focused with
  `LiquidMenu::call_function(uint8_t number) const`.

  @param number - function number used for identification
  @param *function - pointer to the function
  @returns true on success and false if maximum amount of functions
  has been reached

  @note Function numbering starts from 1.
  @note The maximum amount of functions per line is specified in
  LiquidMenu_config.h as `MAX_FUNCTIONS`. The default is 8.

  @see LiquidMenu_config.h
  @see MAX_FUNCTIONS
  @see bool LiquidMenu::call_function(uint8_t number) const
  */
  bool attach_function(uint8_t number, void (*function)(void));

  /// Sets the decimal places for floating point variables.
  /**

  @param decimalPlaces - number of decimal places to show
  */
  void set_decimalPlaces(uint8_t decimalPlaces);

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
  specifying the position.

  @see Position
  */
  bool set_focusPosition(Position position,
                         uint8_t column = 0, uint8_t row = 0);

  /// Converts a byte variable into a glyph index.
  /**
  If a custom character (glyph) was created using
  `DisplayClass::createChar(byte index, byte character[8])` it
  can be displayed as a normal variable using this method.

  @param number - the variable number that will be converted to an index
  @returns true on success and false if the variable with that number is
  not a `byte`.
  */
  bool set_asGlyph(uint8_t number);

  /// Converts a const char pointer variable into const char pointer PROGMEM one.
  /**
  Use this function to tell the object that the attached const char pointer
  variable is saved in flash memory rather than in RAM like a normal variable.

  @param number - the variable number that will be converted
  @returns true on success and false if the variable with that number is
  not a `const char[]`.
  */
  bool set_asProgmem(uint8_t number);
  ///@}

private:
  /// Prints the line to the display.
  /**
  Sets the cursor to the starting position. Then goes through a loop
  calling `print_variable(DisplayClass *p_liquidCrystal, uint8_t number)`.
  And finally displays the focus indicator if the line is focused.

  @param *p_liquidCrystal - pointer to the DisplayClass object
  @param isFocused - true if this line is focused
  */
  void print(DisplayClass *p_liquidCrystal, bool isFocused);

  /// Prints a variable to the display.
  /**
  Casts the variable pointer specified by the number to its data type
  and prints it to the display.

  @param *p_liquidCrystal - pointer to the DisplayClass object
  @param number - number identifying the variable
  */
  void print_variable(DisplayClass *p_liquidCrystal, uint8_t number);

  /// Check if there is an attached function at the specified number.
  /**
  @param number - number of the function in the array
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool is_callable(uint8_t number) const;

  /// Calls an attached function specified by the number.
  /**
  @param number - number identifying the function
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool call_function(uint8_t number) const;

  uint8_t _row, _column, _focusRow, _focusColumn;
  Position _focusPosition;
  uint8_t _floatDecimalPlaces;
  uint8_t _variableCount; ///< Count of the variables
  void (*_function[MAX_FUNCTIONS])(void); ///< Pointers to the functions
  const void *_variable[MAX_VARIABLES]; ///< Pointers to the variables
  DataType _variableType[MAX_VARIABLES]; ///< Data type of the variables
  bool _focusable; ///< Determines whether the line is focusable
};


/// Represents a screen shown on the display.
/**
A screen is made up of LiquidLine objects. It holds pointers to them and
calls their functions when it is active. It also knows on which line the
focus is. This classes' objects go into a LiquidMenu object which controls
them. The public methods are for configuration only.

@see LiquidLine
*/
class LiquidScreen {
  friend class LiquidMenu;

public:

  /// @name Constructors
  ///@{

  /// The main constructor.
  /**
  This is the main constructor that gets called every time.
  */
  LiquidScreen();

  /// Constructor for 1 LiquidLine object.
  /**
  @param &liquidLine - pointer to a LiquidLine object
  */
  explicit LiquidScreen(LiquidLine &liquidLine);

  /// Constructor for 2 LiquidLine object.
  /**
  @param &liquidLine1 - pointer to a LiquidLine object
  @param &liquidLine2 - pointer to a LiquidLine object
  */
  LiquidScreen(LiquidLine &liquidLine1, LiquidLine &liquidLine2);

  /// Constructor for 3 LiquidLine object.
  /**
  @param &liquidLine1 - pointer to a LiquidLine object
  @param &liquidLine2 - pointer to a LiquidLine object
  @param &liquidLine3 - pointer to a LiquidLine object
  */
  LiquidScreen(LiquidLine &liquidLine1, LiquidLine &liquidLine2,
               LiquidLine &liquidLine3);

  /// Constructor for 4 LiquidLine object.
  /**
  @param &liquidLine1 - pointer to a LiquidLine object
  @param &liquidLine2 - pointer to a LiquidLine object
  @param &liquidLine3 - pointer to a LiquidLine object
  @param &liquidLine4 - pointer to a LiquidLine object
  */
  LiquidScreen(LiquidLine &liquidLine1, LiquidLine &liquidLine2,
               LiquidLine &liquidLine3, LiquidLine &liquidLine4);

  ///@}

  /// @name Public methods
  ///@{

  /// Adds a LiquidLine object to the screen.
  /**
  @param &liquidLine - pointer to a LiquidLine object
  @returns true on success and false if the maximum amount of lines
  has been reached

  @note The maximum amount of lines per screen is specified in
  LiquidMenu_config.h as `MAX_LINES`. The default is 8.

  @see LiquidMenu_config.h
  @see MAX_LINES
  */
  bool add_line(LiquidLine &liquidLine);

  /// Sets the focus position for the whole screen at once.
  /**
  The valid positions are `LEFT` and `RIGHT`. `CUSTOM` is not valid
  for this function because it needs individual column and row for
  every line.

  @param position - `LEFT` or `RIGHT`
  @returns true on success and false if the position specified is
  invalid

  @note The `Position` is enum class. Use `Position::(member)` when
  specifying the position.

  @see Position
  */
  bool set_focusPosition(Position position);

  /// Specifies the line size of the display (required for scrolling).
  /**
  This is required when you want to add more lines (LiquidLine
  objects) to a screen (LiquidScreen object) than the display's line
  size. The lines will be scrolled.

  @param lineCount - the line size of the display

  @warning Set this after adding all the "lines" to the "screen"!
  @warning Scrolling currently only works with "focusable" lines!
  */
  void set_displayLineCount(uint8_t lineCount);

  /// Hides the screen.
  /**
  Hiding a screen means that it will be skipped when cycling the
  screens.

  @param hide - true for hiding and false for unhiding

  @note It can still be shown using the `change_screen` methods.

  @see LiquidMenu::change_screen(LiquidScreen p_liquidScreen)
  @see LiquidMenu::change_screen(uint8_t number)
  */
  void hide(bool hide);
  ///@}

private:
  /// Prints the lines pointed by the screen.
  /**
  Calls the `LiquidLine::print(DisplayClass *p_liquidCrystal, bool isFocused)`
  for every line pointed by the screen.

  @param *p_liquidCrystal - pointer to the DisplayClass object
  */
  void print(DisplayClass *p_liquidCrystal) const;

  /// Switches the focus.
  /**
  Switches the focus to the next or previous line
  according to the passed parameter.

  @param forward - true for forward, false for backward
  */
  void switch_focus(bool forward = true);

  /// Directly select focused line.
  /**
  @param lineIndex - index of the focused line
  @return true: on success, false: when the selected line doesn't exist or it isn't focusable
  */
  bool set_focusedLine(uint8_t lineIndex);

  /// Get the index of the currently focused line.
  /**
  @returns the index of the currently focused line
  */
  uint8_t get_focusedLine() const;

  /// Check if there is an attached function at the specified number.
  /**
  @param number - number of the function in the array
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool is_callable(uint8_t number) const;

  /// Calls an attached function specified by the number.
  /**
  Calls the function specified by the number argument for the focused line.

  @param number - number of the function in the array
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool call_function(uint8_t number) const;

  LiquidLine *_p_liquidLine[MAX_LINES]; ///< The LiquidLine objects
  uint8_t _lineCount; ///< Count of the LiquidLine objects
  uint8_t _focus; ///< Index of the focused line
  uint8_t _displayLineCount; ///< The number of lines the display supports
  bool _hidden; ///< If hidden skips this screen when cycling
};


/// Represents a collection of screens forming a menu.
/**
A menu is made up of LiquidScreen objects. It holds pointers to them and
calls their functions depending on which one is active. This is the class
used for control. It is possible to use multiple menus, it that case this
classes' objects go into a LiquidSystem object which controls them using
the same public methods.

@see LiquidScreen
*/
class LiquidMenu {
  friend class LiquidSystem;

public:

  /// @name Constructors
  ///@{

  /// The main constructor.
  /**
  This is the main constructor that gets called every time.

  @param &liquidCrystal - pointer to the DisplayClass object
  @param startingScreen - the number of the screen that will be shown
  first
  */
  LiquidMenu(DisplayClass &liquidCrystal, uint8_t startingScreen = 1);

  /// Constructor for 1 LiquidScreen object.
  /**
  @param &liquidCrystal - pointer to the DisplayClass object
  @param &liquidScreen - pointer to a LiquidScreen object
  @param startingScreen - the number of the screen that will be shown
  first
  */
  LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen,
             uint8_t startingScreen = 1);

  /// Constructor for 2 LiquidScreen objects.
  /**
  @param &liquidCrystal - pointer to the DisplayClass object
  @param &liquidScreen1 - pointer to a LiquidScreen object
  @param &liquidScreen2 - pointer to a LiquidScreen object
  @param startingScreen - the number of the screen that will be shown
  first
  */
  LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen1,
             LiquidScreen &liquidScreen2, uint8_t startingScreen = 1);

  /// Constructor for 3 LiquidScreen objects.
  /**
  @param &liquidCrystal - pointer to the DisplayClass object
  @param &liquidScreen1 - pointer to a LiquidScreen object
  @param &liquidScreen2 - pointer to a LiquidScreen object
  @param &liquidScreen3 - pointer to a LiquidScreen object
  @param startingScreen - the number of the screen that will be shown
  first
  */
  LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen1,
             LiquidScreen &liquidScreen2, LiquidScreen &liquidScreen3,
             uint8_t startingScreen = 1);

  /// Constructor for 4 LiquidScreen objects.
  /**
  @param &liquidCrystal - pointer to the DisplayClass object
  @param &liquidScreen1 - pointer to a LiquidScreen object
  @param &liquidScreen2 - pointer to a LiquidScreen object
  @param &liquidScreen3 - pointer to a LiquidScreen object
  @param &liquidScreen4 - pointer to a LiquidScreen object
  @param startingScreen - the number of the screen that will be shown
  first
  */
  LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen1,
             LiquidScreen &liquidScreen2, LiquidScreen &liquidScreen3,
             LiquidScreen &liquidScreen4, uint8_t startingScreen = 1);

  ///@}

  /// @name Public methods
  ///@{

  /// Adds a LiquidScreen object to the menu.
  /**
  @param &liquidScreen - pointer to a LiquidScreen object
  @returns true on success and false if the maximum amount of screens
  has been reached

  @note The maximum amount of screens per menu is specified in
  LiquidMenu_config.h as `MAX_SCREENS`. The default is 16.

  @see LiquidMenu_config.h
  @see MAX_SCREENS
  */
  bool add_screen(LiquidScreen &liquidScreen);

  /// Returns a reference to the current screen.
  /**
  Call this method to obtain a reference to the current screen.

  @returns a pointer to the current screen.
  */
  LiquidScreen* get_currentScreen() const;

  /// Switches to the next screen.
  void next_screen();

  /// Switches to the next screen.
  /**
  @note Prefix increment operator overloading.
  */
  void operator++();

  /// Switches to the next screen.
  /**
  @note Postfix increment operator overloading.
  */
  void operator++(int);

  /// Switches to the previous screen.
  void previous_screen();

  /// Switches to the previous screen.
  /**
  @note Prefix decrement operator overloading.
  */
  void operator--();

  /// Switches to the previous screen.
  /**
  @note Postfix decrement operator overloading.
  */
  void operator--(int);

  /// Switches to the specified screen.
  /**
  @param *p_liquidScreen - pointer to the LiquidScreen object
  @returns true on success and false if the screen is not found
  */
  bool change_screen(LiquidScreen *p_liquidScreen);

  /// Switches to the specified screen.
  /**
  @param number - the number of the screen
  @returns true on success and false if the
  number of the screen is invalid.
  */
  bool change_screen(uint8_t number);

  /// Switches to the specified screen.
  /**
  @param &p_liquidScreen - pointer to the screen
  @returns true on success and false if the screen is not found
  */
  bool operator=(LiquidScreen *p_liquidScreen);

  /// Switches to the specified screen.
  /**
  @param number - the number of the screen
  @returns true on success and false if the
  number of the screen is invalid.
  */
  bool operator=(uint8_t number);

  /// Switches the focus
  /**
  Switches the focus to the next or previous line
  according to the passed parameter.

  @param forward - true for forward, false for backward
  */
  void switch_focus(bool forward = true);
  
  /// Directly select focused line.
  /**
  @param lineIndex - index of the focused line
  @return true: on success, false: when the selected line doesn't exist or it isn't focusable
  */
  bool set_focusedLine(uint8_t lineIndex);

  /// Get the index of the currently focused line.
  /**
  @returns the index of the currently focused line
  */
  uint8_t get_focusedLine() const;

  /// Sets the focus position for the whole menu at once.
  /**
  The valid positions are `LEFT` and `RIGHT`. `CUSTOM` is not valid
  for this function because it needs individual column and row for
  every line.

  @param position - `LEFT` or `RIGHT`
  @returns true on success and false if the position specified is
  invalid

  @note The `Position` is enum class. Use `Position::(member)` when
  specifying the position.

  @see Position
  */
  bool set_focusPosition(Position position);

  /// Changes the focus indicator's symbol.
  /**
  The symbol is changed for a particular position.

  @param position - the position for which the symbol will be changed
  @param symbol[] - the symbol
  @returns true on success and false if the position specified is
  invalid

  @note The `Position` is enum class. Use `Position::(member)` when
  specifying the position.

  @see Position
  */
  bool set_focusSymbol(Position position, uint8_t symbol[8]);

  /// Check if there is an attached function at the specified number.
  /**
  @param number - number of the function in the array
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool is_callable(uint8_t number) const;

  /// Calls an attached function specified by the number.
  /**
  Calls the function specified by the number argument for the current
  screen and for the focused line.

  @param number - number of the function in the array
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool call_function(uint8_t number) const;

  /// Prints the current screen to the display.
  /**
  Call this method when there is a change in some of the attached variables.
  */
  void update() const;

  /// Prints the current screen to the display (without clearing).
  /**
  Call this method when there is a change in some of the variable attached
  and the new symbols cover all of the old symbols.

  @note This method doesn't clear the display.
  */
  void softUpdate() const;

  /// Initializes the menu object.
  /**
  Call this method to fully initialize the menu object.

  @note Needed when using an I2C display library.
  */
  void init() const;

  ///@}

private:
  DisplayClass *_p_liquidCrystal; ///< Pointer to the DisplayClass object
  LiquidScreen *_p_liquidScreen[MAX_SCREENS]; ///< The LiquidScreen objects
  uint8_t _screenCount; ///< Count of the LiquidScreen objects
  uint8_t _currentScreen;
};


/// Represents a collection of menus forming a menu system.
/**
A menu system is made up of LiquidMenu objects. It holds pointers to them
and calls their functions depending on which one is active. This class is
uses the same public methods as LiquidMenu with the addition of a method
for adding a LiquidMenu object and a method for changing the currently
active menu. This class is optional, it is used only if there is a need
for multiple menus.

@see LiquidMenu
*/
class LiquidSystem {
public:

  /// @name Constructors
  ///@{

  /// The main constructor.
  /**
  This is the main constructor that gets called every time.

  @param startingMenu - the number of the menu that will be shown first
  */
  explicit LiquidSystem(uint8_t startingMenu = 1);

  /// Constructor for 2 LiquidMenu objects.
  /**
  @param &liquidMenu1 - pointer to a LiquidMenu object
  @param &liquidMenu2 - pointer to a LiquidMenu object
  @param startingMenu - the number of the menu that will be shown first
  */
  LiquidSystem(LiquidMenu &liquidMenu1, LiquidMenu &liquidMenu2,
               uint8_t startingMenu = 1);

  /// Constructor for 3 LiquidMenu objects.
  /**
  @param &liquidMenu1 - pointer to a LiquidMenu object
  @param &liquidMenu2 - pointer to a LiquidMenu object
  @param &liquidMenu3 - pointer to a LiquidMenu object
  @param startingMenu - the number of the menu that will be shown first
  */
  LiquidSystem(LiquidMenu &liquidMenu1, LiquidMenu &liquidMenu2,
               LiquidMenu &liquidMenu3, uint8_t startingMenu = 1);

  /// Constructor for 4 LiquidMenu objects.
  /**
  @param &liquidMenu1 - pointer to a LiquidMenu object
  @param &liquidMenu2 - pointer to a LiquidMenu object
  @param &liquidMenu3 - pointer to a LiquidMenu object
  @param &liquidMenu4 - pointer to a LiquidMenu object
  @param startingMenu - the number of the menu that will be shown first
  */
  LiquidSystem(LiquidMenu &liquidMenu1, LiquidMenu &liquidMenu2,
               LiquidMenu &liquidMenu3, LiquidMenu &liquidMenu4,
               uint8_t startingMenu = 1);

  ///@}

  /// @name Public methods
  ///@{

  /// Adds a LiquidMenu object to the menu system.
  /**
  @param &liquidMenu - pointer to a LiquidMenu object
  @returns true on success and false if the maximum amount of menus
  has been reached

  @note The maximum amount of menus per menu system is specified in
  LiquidMenu_config.h as `MAX_MENUS`. The default is 12.

  @see LiquidMenu_config.h
  @see MAX_MENUS
  */
  bool add_menu(LiquidMenu &liquidMenu);

  /// Switches to the specified menu.
  /**
  @param *p_liquidMenu - pointer to the LiquidMenu object
  @returns true on success and false if the menu is not found
  */
  bool change_menu(LiquidMenu &p_liquidMenu);

  /// Returns a reference to the current screen.
  /**
  Call this method to obtain a reference to the current screen.

  @returns a pointer to the current screen.
  */
  LiquidScreen* get_currentScreen() const;

  /// Switches to the next screen.
  void next_screen();

  /// Switches to the next screen.
  /**
  @note Prefix increment operator overloading.
  */
  void operator++();

  /// Switches to the next screen.
  /**
  @note Postfix increment operator overloading.
  */
  void operator++(int);

  /// Switches to the previous screen.
  void previous_screen();

  /// Switches to the previous screen.
  /**
  @note Prefix decrement operator overloading.
  */
  void operator--();

  /// Switches to the previous screen.
  /**
  @note Postfix decrement operator overloading.
  */
  void operator--(int);

  /// Switches to the specified screen.
  /**
  @param *p_liquidScreen - pointer to the LiquidScreen object
  @returns true on success and false if the screen is not found
  */
  bool change_screen(LiquidScreen *p_liquidScreen);

  /// Switches to the specified screen.
  /**
  @param number - the number of the screen
  @returns true on success and false if the
  number of the screen is invalid.
  */
  bool change_screen(uint8_t number);

  /// Switches to the specified screen.
  /**
  @param &p_liquidScreen - pointer to the screen
  @returns true on success and false if the screen is not found
  */
  bool operator=(LiquidScreen *p_liquidScreen);

  /// Switches to the specified screen.
  /**
  @param number - the number of the screen
  @returns true on success and false if the
  number of the screen is invalid.
  */
  bool operator=(uint8_t number);

  /// Switches the focus
  /**
  Switches the focus to the next or previous line
  according to the passed parameter.

  @param forward - true for forward, false for backward
  */
  void switch_focus(bool forward = true);

  /// Directly select focused line.
  /**
  @param lineIndex - index of the focused line
  @return true: on success, false: when the selected line doesn't exist or it isn't focusable
  */
  bool set_focusedLine(uint8_t lineIndex);

  /// Get the index of the currently focused line.
  /**
  @returns the index of the currently focused line
  */
  uint8_t get_focusedLine() const;
  
  /// Sets the focus position for the whole menu at once.
  /**
  The valid positions are `LEFT` and `RIGHT`. `CUSTOM` is not valid
  for this function because it needs individual column and row for
  every line.

  @param position - `LEFT` or `RIGHT`
  @returns true on success and false if the position specified is
  invalid

  @note The `Position` is enum class. Use `Position::(member)` when
  specifying the position.

  @see Position
  */
  bool set_focusPosition(Position position);

  /// Changes the focus indicator's symbol.
  /**
  The symbol is changed for a particular position.

  @param position - the position for which the symbol will be changed
  @param symbol[] - the symbol
  @returns true on success and false if the position specified is
  invalid

  @note The `Position` is enum class. Use `Position::(member)` when
  specifying the position.

  @see Position
  */
  bool set_focusSymbol(Position position, uint8_t symbol[8]);

  /// Check if there is an attached function at the specified number.
  /**
  @param number - number of the function in the array
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool is_callable(uint8_t number) const;

  /// Calls an attached function specified by the number.
  /**
  Calls the function specified by the number argument for the current
  screen and for the focused line.

  @param number - number of the function in the array
  @returns true if there is a function at the specified number

  @note Function numbering starts from 1.

  @see bool LiquidLine::attach_function(uint8_t number, void (*function)(void))
  */
  bool call_function(uint8_t number) const;

  /// Prints the current screen to the display.
  /**
  Call this method when there is a change in some of the variable attached.
  */
  void update() const;

  /// Prints the current screen to the display (without clearing).
  /**
  Call this method when there is a change in some of the variable attached
  and the new symbols cover all of the old symbols.

  @note This method doesn't clear the display.
  */
  void softUpdate() const;

  ///@}

private:
  LiquidMenu *_p_liquidMenu[MAX_MENUS]; ///< The LiquidMenu objects
  uint8_t _menuCount; ///< Count of the LiquidMenu objects
  uint8_t _currentMenu;
};
