#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <EEPROM.h>
#include "Button.h"

// An array holding linearized 8bit PWM values.
const uint8_t log_8bit[11] PROGMEM = {0, 1, 2, 3, 6, 12, 22, 40, 74, 138, 255};

// PWM function using the above array.
void PWM_log(uint8_t pin, uint8_t level) {
	analogWrite(pin, pgm_read_byte_near(log_8bit + level));
}

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
Button up(6, pullup);
Button down(12, pullup);
Button enter(8, pullup);

// Pin definitions and variables for their state.
const byte pin9 = 9;
byte pin9_level = 0;

const byte pin10 = 10;
byte pin10_level = 0;

const byte pinA1 = A4;
byte pinA1_value = 0;

const byte pinA2 = A5;
byte pinA2_value = 0;

// The analog reading sample period in seconds.
// It is later overwritten by it's EEPROM value.
unsigned short sample_period = 2;

// Text used for indication for the save line.
char* saved_text;

enum FunctionTypes {
	increase = 1,
	decrease = 2,
};


// A LiquidLine object can be used more that once.
LiquidLine back_line(11, 1, "-BACK");


LiquidLine welcome_line1(1, 0, "LiquidMenu ", VERSION);
LiquidLine welcome_line2(1, 1, "System example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine mm_line(3, 0, "Main menu");
LiquidScreen mm_screen(mm_line);

// These lines direct to other menus.
LiquidLine outputs_line(0, 0, "-Outputs");
LiquidLine inputs_line(0, 1, "-Inputs");
LiquidScreen io_screen(outputs_line, inputs_line);

// This is the first menu.
LiquidMenu main_menu(lcd, welcome_screen, mm_screen, io_screen, 1);


LiquidLine pin9_line(0, 0, "Pin 9: ", pin9_level);
LiquidLine pin10_line(0, 1, "Pin 10: ", pin10_level);
LiquidScreen pins9_10_screen(pin9_line, pin10_line);

LiquidLine oSave_line(0, 0, "Save values", saved_text);
LiquidScreen oSecondary_screen(oSave_line, back_line);

// This is a second menu object.
LiquidMenu outputs_menu(lcd, pins9_10_screen, oSecondary_screen);


LiquidLine pinA1_line(0, 0, "Pin A4: ", pinA1_value);
LiquidLine pinA2_line(0, 1, "Pin A5: ", pinA2_value);
LiquidScreen pinsA1_A2_screen(pinA1_line, pinA2_line);

LiquidLine iSample_line(0, 0, "Sample: ", sample_period, "s");
LiquidScreen iSecondary_screen(iSample_line, back_line);

// And this is the final third menu object.
LiquidMenu inputs_menu(lcd, pinsA1_A2_screen, iSecondary_screen);

/*
 * LiquidSystem object combines the LiquidMenu objects to form
 * a menu system. It provides the same functions as LiquidMenu
 * with the addition of add_menu() and change_menu().
 */
LiquidSystem menu_system(main_menu, outputs_menu, inputs_menu);


// Checks all the buttons.
void buttonsCheck() {
	if (right.check() == LOW) {
		menu_system.next_screen();
	}
	if (left.check() == LOW) {
		menu_system.previous_screen();
	}
	if (up.check() == LOW) {
		menu_system.call_function(increase);
	}
	if (down.check() == LOW) {
		menu_system.call_function(decrease);
	}
	if (enter.check() == LOW) {
		menu_system.switch_focus();
	}
}

// Callback function that will be attached to back_line.
void go_back() {
	// This function takes reference to the wanted menu.
	menu_system.change_menu(main_menu);
}

void goto_outputs_menu() {
	menu_system.change_menu(outputs_menu);
}

void goto_inputs_menu() {
	menu_system.change_menu(inputs_menu);
}

void increase_pin9() {
	if (pin9_level < 10) {
		pin9_level++;
	} else {
		pin9_level = 0;
	}
	PWM_log(pin9, pin9_level);
	saved_text = (char*)"  ";
}

void decrease_pin9() {
	if (pin9_level > 0) {
		pin9_level--;
	} else {
		pin9_level = 10;
	}
	PWM_log(pin9, pin9_level);
	saved_text = (char*)"  ";
}

void increase_pin10() {
	if (pin10_level < 10) {
		pin10_level++;
	} else {
		pin10_level = 0;
	}
	PWM_log(pin10, pin10_level);
	saved_text = (char*)"  ";
}

void decrease_pin10() {
	if (pin10_level > 1) {
		pin10_level--;
	} else {
		pin10_level = 10;
	}
	PWM_log(pin10, pin10_level);
	saved_text = (char*)"  ";
}

void save_state() {
	EEPROM.put(9, pin9_level);
	EEPROM.put(10, pin10_level);
	EEPROM.put(11, sample_period);
	saved_text = (char*)" *";
}

void increase_samplePeriod() {
	if (sample_period < 10) {
		sample_period++;
	saved_text = (char*)"  ";
	}
}

void decrease_samplePeriod() {
	if (sample_period > 0) {
		sample_period--;
	saved_text = (char*)"  ";
	}
}

void setup() {
	Serial.begin(250000);

	pinMode(pin9, OUTPUT);
	pinMode(pin10, OUTPUT);

    // Reads the values recorded in the EEPROM
	EEPROM.get(9, pin9_level);
	EEPROM.get(10, pin10_level);
	EEPROM.get(11, sample_period);
	PWM_log(pin9, pin9_level);
	PWM_log(pin10, pin10_level);

	lcd.begin(16, 2);

	back_line.set_focusPosition(Position::LEFT);

	back_line.attach_function(1, go_back);

	outputs_line.attach_function(1, goto_outputs_menu);
	outputs_line.attach_function(2, goto_outputs_menu);
	inputs_line.attach_function(1, goto_inputs_menu);
	inputs_line.attach_function(2, goto_inputs_menu);

	pin9_line.attach_function(increase, increase_pin9);
	pin9_line.attach_function(decrease, decrease_pin9);
	pin10_line.attach_function(increase, increase_pin10);
	pin10_line.attach_function(decrease, decrease_pin10);

	oSave_line.attach_function(1, save_state);
	iSample_line.attach_function(increase, increase_samplePeriod);
	iSample_line.attach_function(decrease, decrease_samplePeriod);

	menu_system.update();
}

void loop() {
	buttonsCheck();

	static unsigned long lastMillis_sample = 0;
	if (millis() - lastMillis_sample > (sample_period * 1000)) {
		lastMillis_sample = millis();
		pinA1_value = analogRead(pinA1);
		pinA2_value = analogRead(pinA2);
		menu_system.update();
	}

}
