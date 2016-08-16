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

LiquidSystem::LiquidSystem(uint8_t startingMenu)
	: _menuCount(0), _currentMenu(startingMenu - 1) {

}

LiquidSystem::LiquidSystem(LiquidMenu &liquidMenu1, LiquidMenu &liquidMenu2,
                           uint8_t startingMenu)
	: LiquidSystem(startingMenu) {
	add_menu(liquidMenu1);
	add_menu(liquidMenu2);
}

LiquidSystem::LiquidSystem(LiquidMenu &liquidMenu1, LiquidMenu &liquidMenu2,
                           LiquidMenu &liquidMenu3, uint8_t startingMenu)
	: LiquidSystem(liquidMenu1, liquidMenu2, startingMenu) {
	add_menu(liquidMenu3);
}

LiquidSystem::LiquidSystem(LiquidMenu &liquidMenu1, LiquidMenu &liquidMenu2,
                           LiquidMenu &liquidMenu3, LiquidMenu &liquidMenu4,
                           uint8_t startingMenu)
	: LiquidSystem(liquidMenu1, liquidMenu2, liquidMenu3, startingMenu) {
	add_menu(liquidMenu4);
}


bool LiquidSystem::add_menu(LiquidMenu &liquidMenu) {
	print_me((uintptr_t)this);
	if (_menuCount < MAX_MENUS) {
		_p_liquidMenu[_menuCount] = &liquidMenu;
		DEBUG(F("Added a new menu (")); DEBUG(_menuCount); DEBUGLN(F(")"));
		_menuCount++;
		return true;
	}
	DEBUG(F("Adding menu ")); DEBUG(_menuCount);
	DEBUGLN(F(" failed, edit LiquidMenu_config.h to allow for more menus"));
	return false;
}


bool LiquidSystem::change_menu(LiquidMenu &p_liquidMenu) {
	// _p_liquidMenu[_currentMenu]->_p_liquidCrystal->clear();
	for (uint8_t m = 0; m < _menuCount; m++) {
		if ((uintptr_t)&p_liquidMenu == (uintptr_t) & (*_p_liquidMenu[m])) {
			_currentMenu = m;
			DEBUG(F("Menu changed to ")); DEBUGLN(_currentMenu);
			update();
			return true;
		}
	}
	DEBUG(F("Invalid request for menu change to ")); DEBUGLN((uintptr_t)&p_liquidMenu);
	return false;
}

void LiquidSystem::next_screen() {
	_p_liquidMenu[_currentMenu]->next_screen();
}

void LiquidSystem::operator++() {
	next_screen();
}

void LiquidSystem::operator++(int) {
	next_screen();
}

void LiquidSystem::previous_screen() {
	_p_liquidMenu[_currentMenu]->previous_screen();
}

void LiquidSystem::operator--() {
	previous_screen();
}

void LiquidSystem::operator--(int) {
	previous_screen();
}

bool LiquidSystem::change_screen(uint8_t number) {
	return _p_liquidMenu[_currentMenu]->change_screen(number);
}

bool LiquidSystem::change_screen(LiquidScreen &p_liquidScreen) {
	return _p_liquidMenu[_currentMenu]->change_screen(p_liquidScreen);
}

bool LiquidSystem::operator=(uint8_t number) {
	return change_screen(number);
}

bool LiquidSystem::operator=(LiquidScreen &p_liquidScreen) {
	return change_screen(p_liquidScreen);
}

void LiquidSystem::switch_focus(bool forward) {
	_p_liquidMenu[_currentMenu]->switch_focus(forward);
}

bool LiquidSystem::set_focusPosition(Position position) {
	return _p_liquidMenu[_currentMenu]->set_focusPosition(position);
}

bool LiquidSystem::set_focusSymbol(Position position, uint8_t symbol[8]) {
	return _p_liquidMenu[_currentMenu]->set_focusSymbol(position, symbol);
}

bool LiquidSystem::call_function(uint8_t number) const {
	bool returnValue = _p_liquidMenu[_currentMenu]->call_function(number);
	_p_liquidMenu[_currentMenu]->_p_liquidCrystal->clear();
	update();
	return returnValue;
}

void LiquidSystem::update() const {
	_p_liquidMenu[_currentMenu]->update();
}

void LiquidSystem::softUpdate() const {
	_p_liquidMenu[_currentMenu]->softUpdate();
}
