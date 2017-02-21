/*
 * LiquidMenu library - focus_menu.ino
 *
 * This example demonstrates the use of buttons, callback
 * functions and changing text variables.
 *
 * The example creates three screens. The first one displays static
 * information. The second displays the analog reading and the
 * state of the LED on pin 13. And the third one displays the
 * PWM value applied on pin 3. The analog value is read periodically
 * and assigned to the attached variable "const short analogValue".
 * The LED on pin 13 is changing its state periodically and its
 * attached variable is changed also. The PWM output pin 3 is
 * controlled via the callback functions attached to its line:
 * "void pwm_up()" and "void pwm_down()". To call the functions
 * navigate to the third screen using the "left" or "right" buttons,
 * focus the line using the "enter" button and then click the "up" or
 * "down" buttons.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/focus_menu/focus_menu.png
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
 * - Button (left) to Arduino pin 4
 * - Button (right) to Arduino pin 5
 * - Button (up) to Arduino pin 6
 * - Button (down) to Arduino pin 7
 * - Button (enter) to Arduino pin 8
 * - A PWM controlled device (LED...) to Arduino pin 3
 * - An LED to Arduino pin 13 (optional)
 * - some analog input to Arduino pin A5 (unconnected also works)
 *
 * Created July 24, 2016
 * by Vasil Kalchev
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

// Button objects instantiation
const bool pullup = true;
Button left(4, pullup);
Button right(5, pullup);
Button enter(8, pullup);

LiquidLine welcome_line1(1, 0, "LiquidMenu ", VERSION);
LiquidLine welcome_line2(1, 1, "Focus example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine leftFocus_line(1, 0, "Left focus");
LiquidLine rightFocus_line(1, 1, "Right focus");
LiquidScreen leftright_screen(leftFocus_line, rightFocus_line);

LiquidLine customFocus_line(1, 0, "Custom focus");
LiquidScreen customFocus_screen(customFocus_line);

LiquidLine leftF1_line(6, 0, "Left");
LiquidLine leftF2_line(6, 1, "focus");
LiquidScreen leftF12_screen(leftF1_line, leftF2_line);

LiquidMenu menu(lcd);

// Checks all the buttons.
void buttonsCheck() {
	if (right.check() == LOW) {
		menu.next_screen();
	}
	if (left.check() == LOW) {
		menu.previous_screen();
	}
	if (enter.check() == LOW) {
		// Switches focus to the next line.
		menu.switch_focus();
	}
}

// Indicator symbol definition, it will change the default symbol for the right focus.
// http://omerk.github.io/lcdchargen/
uint8_t rFocus[8] = {
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

    // Changes the right focus symbol.
	menu.set_focusSymbol(Position::RIGHT, rFocus);

    // Sets the focus position for this line to be on the left.
	leftFocus_line.set_focusPosition(Position::LEFT);
	rightFocus_line.set_focusPosition(Position::RIGHT);
	// 'CUSTOM' focus position needs column and row parameters.
	customFocus_line.set_focusPosition(Position::CUSTOM, 15, 0);
	// The focus position can be changed for the whole screen or
	// menu at once. 'CUSTOM' position cannot be set because it
	// needs individual coordinates.
	leftF12_screen.set_focusPosition(Position::LEFT);

    // Attaching a function to a line makes the line focusable.
	leftFocus_line.attach_function(1, func);
	rightFocus_line.attach_function(1, func);
	customFocus_line.attach_function(1, func);
	leftF1_line.attach_function(1, func);
	leftF2_line.attach_function(1, func);

	menu.add_screen(welcome_screen);
	menu.add_screen(leftright_screen);
	menu.add_screen(customFocus_screen);
	menu.add_screen(leftright_screen);
	menu.add_screen(leftF12_screen);

	menu.update();
}

void loop() {
	buttonsCheck();

}
