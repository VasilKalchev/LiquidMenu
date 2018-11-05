/*
 * LiquidMenu library - scrolling_menu.ino
 *
 * This example demonstrates how to use scrolling lines and
 * overwrite the default number of displayed decimal places.
 *
 * It creates 5 "line" obects and places them in a single
 * "screen" object. The requirements for scrolling to work
 * are: the "line" objects must be focusable and the number
 * of lines the actual display has must be set (this also
 * must be set after adding the "lines" to the "screen").
 *
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/J_scrolling_menu/J_scrolling_menu.png
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
//LCD R/W pin to ground
//10K potentiometer to VO
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/*
 * The Button class is not a part of the LiquidMenu library. The first
 * parameter is the button's pin, the second enables or disables the
 * internal pullup resistor (not required) and the third is the debounce
 * time (not required).
 */
// Button objects instantiation
const bool pullup = true;
Button enter(10, pullup);

const float pi = 3.14159265f;

LiquidLine line1(0, 0, pi);
LiquidLine line2(0, 1, "Line 2");
LiquidLine line3(0, 1, "Line 3");
LiquidLine line4(0, 1, "Line 4");
LiquidLine line5(0, 1, "Line 5");

LiquidScreen screen;

LiquidMenu menu(lcd);

// Used for attaching something to the lines, to make them focusable.
void blankFunction() {
    return;
}


void setup() {
    Serial.begin(250000);

	lcd.begin(16, 2);

    // Add more "lines" than the display has. The extra will be scrolled.
    screen.add_line(line1);
    screen.add_line(line2);
    screen.add_line(line3);
    screen.add_line(line4);
    screen.add_line(line5);

    // Attaching a function to the lines is required for scrolling to work.
    line1.attach_function(1, blankFunction);
    line2.attach_function(1, blankFunction);
    line3.attach_function(1, blankFunction);
    line4.attach_function(1, blankFunction);
    line5.attach_function(1, blankFunction);

    // Set the number of lines the display has.
    /*
     * Setting this is a must when you want to add more "lines" than
     * the display actually has (the extra lines will be scrolled).
     * This must be set after adding the "lines" to the "screen".
     */
    screen.set_displayLineCount(2);

    menu.add_screen(screen);

    // Set the number of decimal places for a "line".
    line1.set_decimalPlaces(5);

    menu.update();
}

void loop() {
    if (enter.check() == LOW) {
        Serial.println(F("ENTER button pressed"));
        menu.switch_focus();
		menu.update();
    }
}
