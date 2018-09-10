/*
* LiquidMenu library - focus_menu.ino
*
* This example demonstrates how to customize the focus indicator.
*
* The default focus indicator is an arrow symbol shown on the right of the
* focused line. The focus indicator position can be changed per line to be
* shown on the right, on the left or in a specified position. The focus
* symbol can also be changed, per position.
* In this example a custom focus symbol is defined (uint8_t rFocus[8]). Then
* it is configured to be the right focus indicator symbol using the
* bool LiquidMenu::set_focusSymbol(Position position, uint8_t symbol[8]) method.
* The focus position for the different lines is configured using the
* bool set_focusPosition(Position position, uint8_t column = 0, uint8_t row = 0)
* method. If the "position" is "Position::CUSTOM", the column and row parameters
* must be specified. The focus position can be specified for a single line, for
* the whole screen, for the whole menu or for the whole menu system using the
* respective objects.
*
* The circuit:
* https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/focus_menu/F_focus_menu.png
* - LCD RS pin to Arduino pin 12
* - LCD E pin to Arduino pin 11
* - LCD D4 pin to Arduino pin 5
* - LCD D5 pin to Arduino pin 4
* - LCD D6 pin to Arduino pin 3
* - LCD D7 pin to Arduino pin 2
* - LCD R/W pin to ground
* - LCD VSS pin to ground
* - LCD VCC pin to  5V
* - 10k ohm potentiometer: ends to 5V and ground, wiper to LCD V0
* - 150 ohm resistor from 5V to LCD Anode
* - LCD Cathode to ground
* - ----
* - Button (left) to Arduino pin A0 and ground
* - Button (right) to Arduino pin 7 and ground
* - Button (enter) to Arduino pin 10 and ground
*
* Created July 24, 2016
* by Vasil Kalchev
*
* https://github.com/VasilKalchev/LiquidMenu
* http://omerk.github.io/lcdchargen/ - for creating custom indicator symbol
*
*/

#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include "Button.h"

// Pin mapping for the display
const byte LCD_RS = 12;
const byte LCD_E = 11;
const byte LCD_D4 = 5;
const byte LCD_D5 = 4;
const byte LCD_D6 = 3;
const byte LCD_D7 = 2;
//LCD R/W pin to ground
//10K potentiometer to VO
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

float mnuVar = 2.123456; //float value to test decimal places

// Button objects instantiation
const bool pullup = true;
Button left(A0, pullup);
Button right(7, pullup);
Button enter(10, pullup);

LiquidLine main_menu_1(1, 0, "Item 1:  ", mnuVar);
LiquidLine main_menu_2(1, 1, "Item 2       ");
LiquidLine main_menu_3(1, 2, "Item 3       ");
LiquidLine main_menu_4(1, 3, "Item 4       ");
LiquidLine main_menu_5(1, 4, "Item 5       ");
LiquidLine main_menu_6(1, 5, "Item 6       ");
LiquidLine main_menu_7(1, 6, "Item 7       ");
LiquidLine main_menu_8(1, 7, "End       ");

LiquidScreen screen1 = LiquidScreen();

LiquidMenu menu(lcd);

// Checks all the buttons.
void buttonsCheck() {
	if (enter.check() == LOW) {
		// Switches focus to the next line.
		menu.switch_focus();
	}
}

// Indicator symbol definition, it will change the default symbol for the right focus.
// http://omerk.github.io/lcdchargen/
uint8_t lFocus[8] = {
	0b00000,
	0b00000,
	0b00100,
	0b01110,
	0b00100,
	0b00000,
	0b00000,
	0b00000
};

// Blank function, it is attached to the lines so that they become focusable.
void func() {
	return;
}

void setup() {
	Serial.begin(250000);

	lcd.begin(16, 2);

	// Add Menu Lines
	screen1.add_line(main_menu_1);
	screen1.add_line(main_menu_2);
	screen1.add_line(main_menu_3);
	screen1.add_line(main_menu_4);
	screen1.add_line(main_menu_5);
	screen1.add_line(main_menu_6);
	screen1.add_line(main_menu_7);
	screen1.add_line(main_menu_8);

	// Changes the right focus symbol.
	menu.set_focusSymbol(Position::LEFT, lFocus);

	// Sets the focus position for this line to be on the left.
	main_menu_1.set_focusPosition(Position::LEFT);
	main_menu_2.set_focusPosition(Position::LEFT);
	main_menu_3.set_focusPosition(Position::LEFT);
	main_menu_4.set_focusPosition(Position::LEFT);
	main_menu_5.set_focusPosition(Position::LEFT);
	main_menu_6.set_focusPosition(Position::LEFT);
	main_menu_7.set_focusPosition(Position::LEFT);
	main_menu_8.set_focusPosition(Position::LEFT);

	// Set Menu 1 Line to print float decimal places to 3
	main_menu_1.set_decimal_places(3);

	// Attaching a function to a line makes the line focusable.
	main_menu_1.attach_function(1, func);
	main_menu_2.attach_function(1, func);
	main_menu_3.attach_function(1, func);
	main_menu_4.attach_function(1, func);
	main_menu_5.attach_function(1, func);
	main_menu_6.attach_function(1, func);
	main_menu_7.attach_function(1, func);
	main_menu_8.attach_function(1, func);

	// Set Screen1 to only show a max of 2 lines (to match what our LCD is capable of)
	// Remain screen menu lines will scroll when reaching the bottom
	screen1.set_max_line_display(2);

	menu.add_screen(screen1); // Screen 1

	menu.update();
}

void loop() {
	buttonsCheck();
}
