/*
 * LiquidMenu library - G_scrolling_menu.ino
 *
 * Fitting more lines on a screen than the display physically has, and
 * setting the number of printed decimal places.
 *
 * A LiquidScreen can hold more lines than the display can show (up to
 * MAX_LINES, see "LiquidMenu_config.h"). The extra lines are scrolled
 * into view as the focus moves through them. Two things are required:
 *
 *   - every line must be focusable, i.e. have a function attached,
 *   - the screen must be told how many lines the display really has,
 *     with screen.set_displayLineCount(count), AFTER the lines have
 *     been added to it.
 *
 * This example also shows LiquidLine::set_decimalPlaces(), which
 * overrides how many digits a float or double variable is printed with
 * (the default is 2).
 *
 * This sketch is written for an Arduino Nano, but the pin numbers are
 * the same on an Uno.
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
 * - Button (enter) to Arduino pin 10 and ground
 *
 * Created September 10, 2018
 * by Richard Wardlow - https://github.com/circuitsforfun
 *
 * https://github.com/VasilKalchev/LiquidMenu
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

// The number of lines the display physically has.
const byte DISPLAY_LINES = 2;

// The Button class is not a part of LiquidMenu (see "Button.h").
Button enter(10);

const float pi = 3.14159265f;

LiquidLine line1(0, 0, "Pi: ", pi);
LiquidLine line2(0, 1, "Line 2");
LiquidLine line3(0, 1, "Line 3");
LiquidLine line4(0, 1, "Line 4");
LiquidLine line5(0, 1, "Line 5");

// The lines are added in setup(), so the screen starts out empty.
LiquidScreen screen;

LiquidMenu menu(lcd);

// Attaching a function is what makes a line focusable, and only
// focusable lines take part in the scrolling.
void blank_function() {
  return;
}

void setup() {
  Serial.begin(250000);

  lcd.begin(16, DISPLAY_LINES);

  // More lines are added than the display can show - the rest scroll.
  screen.add_line(line1);
  screen.add_line(line2);
  screen.add_line(line3);
  screen.add_line(line4);
  screen.add_line(line5);

  line1.attach_function(1, blank_function);
  line2.attach_function(1, blank_function);
  line3.attach_function(1, blank_function);
  line4.attach_function(1, blank_function);
  line5.attach_function(1, blank_function);

  /*
   * Without this the screen assumes that the display has as many lines
   * as were added to it. It has to be called after adding the lines,
   * because add_line() updates this count.
   */
  screen.set_displayLineCount(DISPLAY_LINES);

  // Print "pi" with five digits after the decimal point instead of two.
  line1.set_decimalPlaces(5);

  menu.add_screen(screen);

  menu.update();
}

void loop() {
  if (enter.pressed()) {
    // Moving the focus past the bottom line scrolls the screen.
    menu.switch_focus();
  }
}
