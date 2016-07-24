
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
