/*
 * LiquidMenu library - buttons_menu.ino
 *
 * This example demonstrates how to use buttons, callback
 * functions and changing text variables.
 *
 * It creates three screens. The first one displays static
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
 * "down" button.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/D_buttons_menu/D_buttons_menu.png
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
 * - Button (up) to Arduino pin 8 and ground
 * - Button (down) to Arduino pin 9 and ground
 * - Button (enter) to Arduino pin 10 and ground
 * - A PWM controlled device (LED...) to Arduino pin 6
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

/*
 * The Button class is not a part of the LiquidMenu library. The first
 * parameter is the button's pin, the second enables or disables the
 * internal pullup resistor (not required) and the third is the debounce
 * time (not required).
 */
// Button objects instantiation
const bool pullup = true;
Button left(A0, pullup);
Button right(7, pullup);
Button up(8, pullup);
Button down(9, pullup);
Button enter(10, pullup);

// Variables used to demonstrate PWM control with callback functions.
/*
 * Variable 'pwmLevel' is later passed to 'pwm_line' object to be
 * displayed and a function is attached to the same LiquidLine object.
 * Functions are attached to different LiquidLine objects with a number
 * for identification e.g. pwm_line.attach_function(2, func); then when
 * menu.call_function(2) is called on some event e.g. button press, the
 * function identified with 2 for the focused line is called. Similar
 * functions (e.g. for incrementing) can be attached to the different
 * LiquidLine objects with the same identification number and then called
 * on some event (e.g. 'UP' button) with menu.call_function(2), that will
 * call only the function for the focused line that is identified with
 * the number 2.
 */
const byte pwmPin = 6;
byte pwmLevel = 0;

// Variables for controlling a pin and displaying the state with text.
// char* is used for adding changing text to the LiquidLine object.
const byte ledPin = LED_BUILTIN;
bool ledState = LOW;
char* ledState_text;

/*
 * Variable 'analogValue' is later configured to be printed on the display.
 * This time a static variable is used for holding the previous analog value.
 */
const byte analogPin = A5;
unsigned short analogValue = 0;


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(0, 1, "Buttons example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine analog_line(0, 0, "Analog: ", analogValue);
LiquidLine ledState_line(0, 1, "LED is ", ledState_text);
LiquidScreen screen2(analog_line, ledState_line);

LiquidLine pwm_line(0, 0, "PWM level: ", pwmLevel);
LiquidScreen pwm_screen(pwm_line);

LiquidMenu menu(lcd);

// Function to be attached to the pwm_line object.
/*
 * This function is used for incrementing the PWM level on the
 * 'pwmPin'. It increments the value of 'pwmLevel' and then
 * writes it to the pin.
 */
void pwm_up() {
	if (pwmLevel < 225) {
		pwmLevel += 25;
	} else {
		pwmLevel = 250;
	}
	analogWrite(pwmPin, pwmLevel);
}

// Function to be attached to the pwm_line object.
void pwm_down() {
	if (pwmLevel > 25) {
		pwmLevel -= 25;
	} else {
		pwmLevel = 0;
	}
	analogWrite(pwmPin, pwmLevel);
}

void setup() {
	Serial.begin(250000);

	pinMode(analogPin, INPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(pwmPin, OUTPUT);

	lcd.begin(16, 2);

	// Function to attach functions to LiquidLine objects.
	pwm_line.attach_function(1, pwm_up);
	pwm_line.attach_function(2, pwm_down);

	menu.add_screen(welcome_screen);
	menu.add_screen(screen2);
	menu.add_screen(pwm_screen);
}

void loop() {

	// Check all the buttons
	if (right.check() == LOW) {
		Serial.println(F("RIGHT button pressed"));
		menu.next_screen();
	}
	if (left.check() == LOW) {
		Serial.println(F("LEFT button pressed"));
		menu.previous_screen();
	}
	if (up.check() == LOW) {
		Serial.println(F("UP button pressed"));
		// Calls the function identified with one
		// for the focused line.
		menu.call_function(1);
	}
	if (down.check() == LOW) {
		Serial.println(F("DOWN button pressed"));
		menu.call_function(2);
	}
	if (enter.check() == LOW) {
		Serial.println(F("ENTER button pressed"));
		// Switches focus to the next line.
		menu.switch_focus();
	}


	// Periodically switches the state of the LED.
	static unsigned long lastMillis = 0;
	static unsigned int period = 2000;
	if (millis() - lastMillis > period) {
		lastMillis = millis();

		Serial.print("LED turned ");
		if (ledState == LOW) {
			ledState = HIGH;
			// Changes the text that is printed on the display.
			ledState_text = (char*)"ON";
			Serial.println(ledState_text);
			menu.update();
		} else {
			ledState = LOW;
			ledState_text = (char*)"OFF";
			Serial.println(ledState_text);
			menu.update();
		}
		digitalWrite(ledPin, ledState);

		// The 'analogValue' is updated every second.
		analogValue = analogRead(analogPin);
		static unsigned short lastAnalogValue = 0;
		if (analogValue != lastAnalogValue) {
			lastAnalogValue = analogValue;
			menu.update();
		}
	}
}
