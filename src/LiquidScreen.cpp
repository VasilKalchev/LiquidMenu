/**
@file
Contains the LiquidScreen class definition.
*/

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


/// Line count subtrahend for comparison during focus iteration
/**
0 - "ghosting" is enabled
1 - "ghosting" is disabled
*/
#define LM_LINE_COUNT_SUBTRAHEND (0)

#if LM_FOCUS_INDICATOR_GHOSTING == true
	#undef LM_LINE_COUNT_SUBTRAHEND
	#define LM_LINE_COUNT_SUBTRAHEND (0)
#elif LM_FOCUS_INDICATOR_GHOSTING == false
	#undef LM_LINE_COUNT_SUBTRAHEND
	#define LM_LINE_COUNT_SUBTRAHEND (1)
#endif



LiquidScreen::LiquidScreen()
	: _lineCount(0), _focus(0), _hidden(false) {}

LiquidScreen::LiquidScreen(LiquidLine &liquidLine)
	: LiquidScreen() {
	add_line(liquidLine);
}

LiquidScreen::LiquidScreen(LiquidLine &liquidLine1, LiquidLine &liquidLine2)
	: LiquidScreen(liquidLine1) {
	add_line(liquidLine2);
}

LiquidScreen::LiquidScreen(LiquidLine &liquidLine1, LiquidLine &liquidLine2,
                           LiquidLine &liquidLine3)
	: LiquidScreen(liquidLine1, liquidLine2) {
	add_line(liquidLine3);
}

LiquidScreen::LiquidScreen(LiquidLine &liquidLine1, LiquidLine &liquidLine2,
                           LiquidLine &liquidLine3, LiquidLine &liquidLine4)
	: LiquidScreen(liquidLine1, liquidLine2, liquidLine3) {
	add_line(liquidLine4);
}

bool LiquidScreen::add_line(LiquidLine &liquidLine) {
	DEBUG(F("LScreen ")); print_me(reinterpret_cast<uintptr_t>(this));

	DEBUG(F("Add line (0x")); DEBUG((uintptr_t)&liquidLine);
	DEBUG(F(") count(")) DEBUG(_lineCount); DEBUG(F(")"));

	if (_lineCount < MAX_LINES) {
		_p_liquidLine[_lineCount] = &liquidLine;
		_lineCount++;

		// set the focus indicator equal to the amount of lines, which
		// effectively makes it invisible on first run
		_focus++;

		// Naively set the number of lines the display has to the
		// number of added LiquidLine objects. When adding more
        // LiquidLine objects that the display's number of lines,
        // void LiquidScreen::set_displayLineCount(uint8_t lines)
        // must be used to set the number of lines the display has.
	    _displayLineCount = _lineCount;

		DEBUGLN(F(""));
		return true;
	} else {
		DEBUGLN(F(" failed, edit LiquidMenu_config.h to allow for more lines"));
		return false;
	}

}

bool LiquidScreen::set_focusPosition(Position position) {
	DEBUG(F("LScreen ")); print_me(reinterpret_cast<uintptr_t>(this));

	if (position == Position::CUSTOM) {
		DEBUGLN(F("Can't set focus position to 'CUSTOM' for the whole screen at once"));
		return false;
	} else {
		DEBUG(F("Focus position set to : ")); DEBUGLN((uint8_t)position);
		for (uint8_t l = 0; l < _lineCount; l++) {
			_p_liquidLine[l]->set_focusPosition(position);
		}
		return true;
	}
}

void LiquidScreen::set_displayLineCount(uint8_t lines) {
	_displayLineCount = lines;
}

void LiquidScreen::hide(bool hide) {
	_hidden = hide;
}

void LiquidScreen::print(DisplayClass *p_liquidCrystal) const {
	uint8_t lOffset = 0;
	uint8_t displayLineCount = _displayLineCount;
	if (displayLineCount == 0) {
		displayLineCount = _lineCount;
	} else if (displayLineCount > _lineCount) {
		displayLineCount = _lineCount;
	}

	if (_focus >= displayLineCount) {
		lOffset = (_focus - displayLineCount) + 1;
		if ((displayLineCount + lOffset) > _lineCount) {
			lOffset = (_lineCount - displayLineCount);
		}
	}
	uint8_t offsetRow = 0;
	for (uint8_t l = lOffset; l < displayLineCount + lOffset; l++) {
		bool focus = true;
		if (_focus != l) {
			focus = false;
			DEBUG(F("|"));
		} else {
			DEBUG(F("|   -->"));
		}
		DEBUG(F("\tLine ")); DEBUG(l);
		if (displayLineCount < _lineCount) {
			_p_liquidLine[l]->_row = offsetRow;
		}
		_p_liquidLine[l]->print(p_liquidCrystal, focus);
		offsetRow++;
	}
}

void LiquidScreen::switch_focus(bool forward) {
	DEBUG(F("LScreen ")); print_me(reinterpret_cast<uintptr_t>(this));

	// LM_LINE_COUNT_SUBTRAHEND:
	// 0 - "ghosting" is enabled
	// 1 - "ghosting" is disabled
	
	do {
		if (forward) {
			if (_focus < (_lineCount - LM_LINE_COUNT_SUBTRAHEND)) {
				_focus++;
				if (_focus == (_lineCount - LM_LINE_COUNT_SUBTRAHEND)) {
					break;
				}
			} else {
				_focus = 0;
			}
		} else { //else (forward)
			if (_focus == 0) {
				_focus = (_lineCount - LM_LINE_COUNT_SUBTRAHEND);
				break;
			} else {
				_focus--;
			}
		} //else (forward)
	} while (_p_liquidLine[_focus]->_focusable == false);

	DEBUG(F("Focus switched to ")); DEBUGLN(_focus);
}

bool LiquidScreen::set_focusedLine(uint8_t lineIndex) {
	if (lineIndex < _lineCount && _p_liquidLine[lineIndex]->_focusable == true) {
		_focus = lineIndex;
		return true;
	} else {
		return false;
	}
}

uint8_t LiquidScreen::get_focusedLine() const {
	return _focus;
}

bool LiquidScreen::is_callable(uint8_t number) const {
	if (_focus != _lineCount) {
		return _p_liquidLine[_focus]->is_callable(number);
	}
	return false;
}

bool LiquidScreen::call_function(uint8_t number) const {
	if (_focus != _lineCount) {
		return _p_liquidLine[_focus]->call_function(number);
	}
	return false;
}
