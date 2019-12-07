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
Contains the LiquidMenu class definition.
*/

#include "LiquidMenu.h"
#include "glyphs.h"

const uint8_t DIVISION_LINE_LENGTH = 40; ///< Sets the length of the division line.

LiquidMenu::LiquidMenu(DisplayClass &liquidCrystal, uint8_t startingScreen)
  : _p_liquidCrystal(&liquidCrystal), _screenCount(0),
    _currentScreen(startingScreen - 1) {
}

LiquidMenu::LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen,
                       uint8_t startingScreen)
  : LiquidMenu(liquidCrystal, startingScreen) {
  add_screen(liquidScreen);
}

LiquidMenu::LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen1,
                       LiquidScreen &liquidScreen2, uint8_t startingScreen)
  : LiquidMenu(liquidCrystal, liquidScreen1, startingScreen) {
  add_screen(liquidScreen2);
}

LiquidMenu::LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen1,
                       LiquidScreen &liquidScreen2, LiquidScreen &liquidScreen3,
                       uint8_t startingScreen)
  : LiquidMenu(liquidCrystal, liquidScreen1, liquidScreen2, startingScreen) {
  add_screen(liquidScreen3);
}

LiquidMenu::LiquidMenu(DisplayClass &liquidCrystal, LiquidScreen &liquidScreen1,
                       LiquidScreen &liquidScreen2, LiquidScreen &liquidScreen3,
                       LiquidScreen &liquidScreen4, uint8_t startingScreen)
  : LiquidMenu(liquidCrystal, liquidScreen1, liquidScreen2, liquidScreen3,
               startingScreen) {
  add_screen(liquidScreen4);
}

bool LiquidMenu::add_screen(LiquidScreen &liquidScreen) {
  print_me(reinterpret_cast<uintptr_t>(this));
  if (_screenCount < MAX_SCREENS) {
    _p_liquidScreen[_screenCount] = &liquidScreen;
    DEBUG(F("Added a new screen (")); DEBUG(_screenCount); DEBUGLN(F(")"));
    _screenCount++;
    return true;
  }
  DEBUG(F("Adding screen ")); DEBUG(_screenCount);
  DEBUGLN(F(" failed, edit LiquidMenu_config.h to allow for more screens"));
  return false;
}

LiquidScreen* LiquidMenu::get_currentScreen() const {
  return _p_liquidScreen[_currentScreen];
}

void LiquidMenu::next_screen() {
  _p_liquidCrystal->clear();
  do {
    if (_currentScreen < _screenCount - 1)  {
      _currentScreen++;
    } else {
      _currentScreen = 0;
    }
  } while (_p_liquidScreen[_currentScreen]->_hidden == true);
  update();
  DEBUG(F("Switched to the next screen (")); DEBUG(_currentScreen); DEBUG(F(")"));
}

void LiquidMenu::operator++() {
  next_screen();
}

void LiquidMenu::operator++(int) {
  next_screen();
}

void LiquidMenu::previous_screen() {
  _p_liquidCrystal->clear();
  do {
    if (_currentScreen > 0) {
      _currentScreen--;
    } else {
      _currentScreen = _screenCount - 1;
    }
  } while (_p_liquidScreen[_currentScreen]->_hidden == true);
  update();
  DEBUG(F("Switched to the previous screen (")); DEBUG(_currentScreen); DEBUGLN(F(")"));
}

void LiquidMenu::operator--() {
  previous_screen();
}

void LiquidMenu::operator--(int) {
  previous_screen();
}

bool LiquidMenu::change_screen(uint8_t number) {
  uint8_t index = number - 1;
  if (index <= _screenCount) {
    _p_liquidCrystal->clear();
    _currentScreen = index;
    update();
    DEBUG(F("Switched to screen ("));
    DEBUG(_currentScreen); DEBUGLN(F(")"));
    return true;
  } else {
    DEBUG(F("Invalid request for screen change to ")); DEBUGLN(number);
    return false;
  }
}

bool LiquidMenu::change_screen(LiquidScreen *p_liquidScreen) {
  // _p_liquidMenu[_currentMenu]->_p_liquidCrystal->clear();
  for (uint8_t s = 0; s < _screenCount; s++) {
    _p_liquidCrystal->clear();
    if (p_liquidScreen == _p_liquidScreen[s]) {
    // if (reinterpret_cast<uintptr_t>(&p_liquidScreen) == reinterpret_cast<uintptr_t>(&(*_p_liquidScreen[s]))) {
      _currentScreen = s;
      update();
      DEBUG(F("Switched to screen ("));
      DEBUG(_currentScreen); DEBUGLN(F(")"));
      update();
      return true;
    }
  }
  DEBUG(F("Invalid request for screen change to 0x")); DEBUGLN(reinterpret_cast<uintptr_t>(&p_liquidScreen));
  return false;
}

bool LiquidMenu::operator=(uint8_t number) {
  return change_screen(number);
}

bool LiquidMenu::operator=(LiquidScreen *p_liquidScreen) {
  return change_screen(p_liquidScreen);
}

void LiquidMenu::switch_focus(bool forward) {
  _p_liquidCrystal->clear();
  _p_liquidScreen[_currentScreen]->switch_focus(forward);
  update();
}

bool LiquidMenu::set_focusedLine(uint8_t lineIndex) {
  return _p_liquidScreen[_currentScreen]->set_focusedLine(lineIndex);
}

uint8_t LiquidMenu::get_focusedLine() const {
  return _p_liquidScreen[_currentScreen]->get_focusedLine();
}

bool LiquidMenu::set_focusPosition(Position position) {
  print_me(reinterpret_cast<uintptr_t>(this));
  if (position == Position::CUSTOM) {
    DEBUGLN(F("Can't set a custom focus position for the whole menu at once"));
    return false;
  } else {
    DEBUG(F("Focus position set to ")); DEBUGLN((uint8_t)position);
    for (uint8_t s = 0; s < _screenCount; s++) {
      _p_liquidScreen[s]->set_focusPosition(position);
    }
    return true;
  }
}

bool LiquidMenu::set_focusSymbol(Position position, uint8_t symbol[8]) {
  switch (position) {
  case Position::RIGHT: {
    _p_liquidCrystal->createChar(15, symbol);
    DEBUG(F("Right"));
    break;
  } //case RIGHT
  case Position::LEFT: {
    _p_liquidCrystal->createChar(14, symbol);
    DEBUG(F("Left"));
    break;
  } //case LEFT
  case Position::CUSTOM: {
    _p_liquidCrystal->createChar(13, symbol);
    DEBUG(F("Custom"));
    break;
  } //case CUSTOM
  default: {
    DEBUGLN(F("Invalid focus position, options are 'RIGHT', 'LEFT' and 'CUSTOM'"));
    return false;
  } //default
  } //switch (position)
  DEBUGLN(F("Focus symbol changed to:"));
  for (uint8_t i = 0; i < 8; i++) {
    DEBUGLN2(symbol[i], BIN);
  }
  return true;
}

bool LiquidMenu::is_callable(uint8_t number) const {
  return _p_liquidScreen[_currentScreen]->is_callable(number);
}

bool LiquidMenu::call_function(uint8_t number) const {
  bool returnValue = _p_liquidScreen[_currentScreen]->call_function(number);
  update();
  return returnValue;
}

void LiquidMenu::update() const {
  _p_liquidCrystal->clear();
  softUpdate();
}

void LiquidMenu::softUpdate() const {
  /* TEMPORARY FIX!
   * Calls methods on the `DisplayClass` object only after it is
   * initialized. This makes it compatible with different
   * "LiquidCrystal" libraries and doesn't break the current API.
   * Will be removed for version 2.0.0 when `LiquidMenu::init()` will
   * become mandatory.
   */
  static bool firstRun = true;
  if (firstRun) {
    firstRun = false;
    _p_liquidCrystal->createChar(15, glyph::rightFocus);
    _p_liquidCrystal->createChar(14, glyph::leftFocus);
    _p_liquidCrystal->createChar(13, glyph::customFocus);
  }

  DEBUGLN(F("Updating the LCD"));
  for (uint8_t b = 0; b < DIVISION_LINE_LENGTH; b++) {
    DEBUG(F("-"));
  }
  DEBUGLN();
  DEBUG(F("|Screen ")); DEBUGLN(_currentScreen);
  _p_liquidScreen[_currentScreen]->print(_p_liquidCrystal);
  for (uint8_t b = 0; b < DIVISION_LINE_LENGTH; b++) {
    DEBUG(F("-"));
  }
  DEBUGLN("\n");
}

void LiquidMenu::init() const {
  _p_liquidCrystal->createChar(15, glyph::rightFocus);
  _p_liquidCrystal->createChar(14, glyph::leftFocus);
  _p_liquidCrystal->createChar(13, glyph::customFocus);
}
