/*
 * LiquidMenu library - E_focus_menu.ino
 *
 * Customizing the focus (selection) indicator.
 *
 * By default the focused line is marked with an arrow shown to its right.
 * Both the position and the symbol can be changed:
 *
 *   line.set_focusPosition(Position::LEFT);
 *   line.set_focusPosition(Position::CUSTOM, column, row);
 *   menu.set_focusSymbol(Position::RIGHT, symbol);
 *
 * The position is a property of a line, but it can be set for a whole
 * screen, a whole menu or a whole menu system at once by calling the
 * method on the respective object (Position::CUSTOM is the exception -
 * it needs coordinates, so it can only be set per line). The symbol is
 * set per position and applies to the whole menu.
 *
 * Remember that only a line with an attached function is focusable,
 * which is why the empty "blank_function" is attached to every line
 * that should take the focus.
 *
 * Simulate it online:
 * Open this folder's "diagram.json" in Wokwi - see ../../README.md.
 *
 * The circuit:
 * - LCD RS pin to Arduino pin 12
 * - LCD E pin to Arduino pin 11
 * - LCD D4 pin to Arduino pin 5
 * - LCD D5 pin to Arduino pin 4
 * - LCD D6 pin to Arduino pin 3
 * - LCD D7 pin to Arduino pin 2
 * - LCD R/W pin to ground
 * - LCD VSS pin to ground
 * - LCD VDD pin to 5V
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
 * http://omerk.github.io/lcdchargen/ - for creating the indicator symbol
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
// LCD R/W pin to ground
// 10k potentiometer wiper to V0
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// The Button class is not a part of LiquidMenu (see "Button.h").
Button left(A0);
Button right(7);
Button enter(10);

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "Focus example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

// This screen shows the two built-in positions side by side.
LiquidLine leftFocus_line(1, 0, "Left focus");
LiquidLine rightFocus_line(1, 1, "Right focus");
LiquidScreen leftRight_screen(leftFocus_line, rightFocus_line);

// This one puts the indicator at a fixed column and row.
LiquidLine customFocus_line(1, 0, "Custom focus");
LiquidScreen customFocus_screen(customFocus_line);

// And this one sets the position for the whole screen at once.
LiquidLine screenFocus_line1(6, 0, "Left");
LiquidLine screenFocus_line2(6, 1, "focus");
LiquidScreen screenFocus_screen(screenFocus_line1, screenFocus_line2);

LiquidMenu menu(lcd, welcome_screen, leftRight_screen,
                customFocus_screen, screenFocus_screen);

/*
 * The symbol that replaces the default right focus indicator. Every byte
 * is one row of the 5x8 character, every bit is one pixel.
 * http://omerk.github.io/lcdchargen/
 */
uint8_t rightFocusSymbol[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};

// Attaching a function is what makes a line focusable - this one is
// attached to the lines that have nothing else to do.
void blank_function() {
  return;
}

void checkButtons() {
  if (right.pressed()) {
    menu.next_screen();
  }
  if (left.pressed()) {
    menu.previous_screen();
  }
  if (enter.pressed()) {
    menu.switch_focus();
  }
}

void setup() {
  Serial.begin(250000);

  lcd.begin(16, 2);

  // Changes the symbol used for the right focus position.
  menu.set_focusSymbol(Position::RIGHT, rightFocusSymbol);

  // Per line...
  leftFocus_line.set_focusPosition(Position::LEFT);
  rightFocus_line.set_focusPosition(Position::RIGHT);
  // ...'CUSTOM' needs a column and a row.
  customFocus_line.set_focusPosition(Position::CUSTOM, 15, 0);
  // ...and for every line of a screen at once.
  screenFocus_screen.set_focusPosition(Position::LEFT);

  leftFocus_line.attach_function(1, blank_function);
  rightFocus_line.attach_function(1, blank_function);
  customFocus_line.attach_function(1, blank_function);
  screenFocus_line1.attach_function(1, blank_function);
  screenFocus_line2.attach_function(1, blank_function);

  menu.update();
}

void loop() {
  checkButtons();
}
