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

#include "LiquidMenu.h"

//const char NOTHING[] = "                    "; ///< 20 spaces, used to clear the display.

/// TODO: Always shows "Line..."
void print_me(uintptr_t address) {
	DEBUG(F("Line (0x")); DEBUG2(address, OCT); DEBUG(F("): "));
	return;
	address = address;
}


bool LiquidLine::attach_function(uint8_t number, void (*function)(void)) {
	print_me((uintptr_t)this);
	if (number <= MAX_FUNCTIONS) {
		_function[number - 1] = function;
		DEBUG(F("Attached function ")); DEBUGLN(number);
		_focusable = true;
		return true;
	} else {
		DEBUG(F("Attaching function ")); DEBUG(number);
		DEBUGLN(F(" failed, edit LiquidMenu_config.h to allow for more functions"));
		return false;
	}
}

bool LiquidLine::set_focusPosition(Position position, uint8_t column, uint8_t row) {
	print_me((uintptr_t)this);
	if (position <= Position::CUSTOM) {
		_focusPosition = position;
		_focusColumn = column;
		_focusRow = row;
		DEBUG(F("Focus position set to ")); DEBUG((uint8_t)_focusPosition);
		if (_focusPosition == Position::CUSTOM) {
			DEBUG(F(" at (")); DEBUG(_focusRow); DEBUG(F(", "));
			DEBUG(_focusColumn); DEBUG(F(")"));
		}
		DEBUGLN();
		return true;
	} else {
		DEBUGLN(F("Failed setting focus position, options are 'RIGHT', 'LEFT' and 'CUSTOM'"));
		return false;
	}
}

bool LiquidLine::set_asGlyph(uint8_t number) {
	uint8_t index = number - 1;
	if ( (index < MAX_VARIABLES) && (_variableType[index] == DataType::UINT8_T) ) {
		_variableType[index] = DataType::GLYPH;
		return true;
	} else {
		DEBUG(F("Setting variable ")); DEBUG(number);
		DEBUGLN(F(" as glyph failed, the variable must be of 'byte' data type"))
		return false;
	}
}

bool LiquidLine::set_asProgmem(uint8_t number) {
	uint8_t index = number - 1;
	if ((index < MAX_VARIABLES) && (_variableType[index] == DataType::CONST_CHAR_PTR)) {
		_variableType[index] = DataType::PROG_CONST_CHAR_PTR;
		return true;
	}
	else {
		DEBUG(F("Setting variable ")); DEBUG(number);
		DEBUGLN(F(" as PROG_CONST_CHAR failed, the variable must be of 'const char[]' data type"))
			return false;
	}
}

void LiquidLine::print(LiquidCrystal *p_liquidCrystal, bool isFocused) {
	p_liquidCrystal->setCursor(_column, _row);
	DEBUG(F(" (")); DEBUG(_column); DEBUG(F(", ")); DEBUG(_row); DEBUGLN(F(")"));

	DEBUG(F("|\t"));
	for (uint8_t v = 0; v < MAX_VARIABLES; v++) {
		print_variable(p_liquidCrystal, v);
	}
	DEBUGLN();

	if (isFocused) {
		DEBUG(F("\t\t<Focus position: "));
		switch (_focusPosition) {
		case Position::RIGHT: {
			p_liquidCrystal->write((uint8_t)15);
			DEBUGLN(F("right>"));
			//p_liquidCrystal->print(NOTHING);
			break;
		} //case RIGHT
		case Position::LEFT: {
			//p_liquidCrystal->print(NOTHING);
			p_liquidCrystal->setCursor(_column - 1, _row);
			p_liquidCrystal->write((uint8_t)14);
			DEBUGLN(F("left>"));
			break;
		} //case LEFT
		case Position::CUSTOM: {
			//p_liquidCrystal->print(NOTHING);
			p_liquidCrystal->setCursor(_focusColumn, _focusRow);
			p_liquidCrystal->write((uint8_t)13);
			DEBUGLN(F("custom (")); DEBUG(_focusColumn);
			DEBUG(F(", ")); DEBUG(_focusRow); DEBUGLN(F(")>"));
			break;
		} //case CUSTOM
		default: {
			DEBUG(F("invalid (")); DEBUG((uint8_t)_focusPosition);
			DEBUGLN(F("), switching to default>"));
			_focusPosition = Position::NORMAL;
			p_liquidCrystal->write((uint8_t)15);
			//p_liquidCrystal->print(NOTHING);
			break;
		} //default
		} //switch (_focusPosition)
	} else {
		//p_liquidCrystal->print(NOTHING);
	}
}

void LiquidLine::print_variable(LiquidCrystal *p_liquidCrystal, uint8_t number) {
	switch (_variableType[number]) {

	case DataType::CONST_CHAR_PTR: {
		const char* variable = reinterpret_cast<const char*>(_variable[number]);
		DEBUG(F("(const char*)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case CONST_CHAR_PTR
	case DataType::CHAR_PTR: {
		char* variable = *reinterpret_cast<char**>( const_cast<void*>(_variable[number]) );
		DEBUG(F("(char*)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case CHAR_PTR
	case DataType::CHAR: {
		const char variable = *static_cast<const char*>(_variable[number]);
		DEBUG(F("(char)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case CHAR

	case DataType::INT8_T: {
		const int8_t variable = *static_cast<const int8_t*>(_variable[number]);
		DEBUG(F("(int8_t)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case INT8_T
	case DataType::UINT8_T: {
		const uint8_t variable = *static_cast<const uint8_t*>(_variable[number]);
		DEBUG(F("(uint8_t)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case UINT8_T

	case DataType::INT16_T: {
		const int16_t variable = *static_cast<const int16_t*>(_variable[number]);
		DEBUG(F("(int16_t)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case INT16_T
	case DataType::UINT16_T: {
		const uint16_t variable = *static_cast<const uint16_t*>(_variable[number]);
		DEBUG(F("(uint16_t)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case UINT16_T

	case DataType::INT32_T: {
		const int32_t variable = *static_cast<const int32_t*>(_variable[number]);
		DEBUG(F("(int32_t)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case INT32_T
	case DataType::UINT32_T: {
		const uint32_t variable = *static_cast<const uint32_t*>(_variable[number]);
		DEBUG(F("(uint32_t)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case UINT32_T

	case DataType::FLOAT: {
		const float variable = *static_cast<const float*>(_variable[number]);
		DEBUG(F("(float)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case FLOAT

	case DataType::BOOL: {
		const bool variable = *static_cast<const bool*>(_variable[number]);
		DEBUG(F("(bool)")); DEBUG(variable);
		p_liquidCrystal->print(variable);
		break;
	} //case BOOL

	case DataType::GLYPH: {
		const uint8_t variable = *static_cast<const uint8_t*>(_variable[number]);
		DEBUG(F("(glyph)")); DEBUG(variable);
		p_liquidCrystal->write((uint8_t)variable);
		break;
	} //case BOOL

	case DataType::PROG_CONST_CHAR_PTR: {
		const char* variable = reinterpret_cast<const char*>(_variable[number]);
		volatile const int len = strlen_P(variable);
		char buffer[len];
		for (uint8_t i = 0; i < len; i++) {
			buffer[i] = pgm_read_byte_near(variable + i);
		}
		buffer[len] = '\0';
		DEBUG(F("(const char*)")); DEBUG(buffer);
		p_liquidCrystal->print(buffer);
		break;
	} //case PROG_CONST_CHAR_PTR

	default: { break; }

	} //switch (_variableType)
	DEBUG(F(" "));
}

bool LiquidLine::call_function(uint8_t number) const {
	if (_function[number - 1]) {
		(*_function[number - 1])();
		return true;
	} else {
		return false;
	}
}
