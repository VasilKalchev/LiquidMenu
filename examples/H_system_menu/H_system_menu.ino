/*
 * LiquidMenu library - system_menu.ino
 *
 * This example demonstrates the how to build a menu system.
 *
 * Menu system is a collection of menus. To build a menu system
 * you need to put the menus into an object of class LiquidSystem.
 * The methods used on LiquidMenu should now be called directly
 * on the LiquidSystem object. To switch between menus use the
 * bool LiquidSystem::change_menu(LiquidMenu &p_liquidMenu) method.
 * This method can be wrapped inside a callback function and called
 * on through the menu. This example also uses EEPROM to save the
 * state of the LED and reading period.
 *
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/H_system_menu/system_menu.png
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
 * - some analog input to Arduino pin A4 (unconnected also works)
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
#include <EEPROM.h>
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
Button left(A0, pullup);
Button right(7, pullup);
Button up(8, pullup);
Button down(9, pullup);
Button enter(10, pullup);

// Pin definitions and variables for their state.
const byte pin6 = 6;
byte pin6_level = 0;

const byte pinA4 = A4;
byte pinA4_value = 0;

const byte pinA5 = A5;
byte pinA5_value = 0;

// The analog reading sample period in seconds.
// It is later overwritten by it's EEPROM value.
unsigned short sample_period = 2;

// Text used for indication for the save lines.
char* input_saved;
char* output_saved;

enum FunctionTypes {
  increase = 1,
  decrease = 2,
};


// A LiquidLine object can be used more that once.
LiquidLine back_line(11, 1, "/BACK");


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "System example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

// These lines direct to other menus.
LiquidLine outputs_line(0, 0, "/Outputs");
LiquidLine inputs_line(0, 1, "/Inputs");
LiquidScreen io_screen(outputs_line, inputs_line);

// This is the first menu.
LiquidMenu main_menu(lcd, welcome_screen, io_screen, 1);


LiquidLine pin6_line(0, 0, "Pin 6: ", pin6_level);
LiquidScreen pin6_screen(pin6_line);

LiquidLine oSave_line(0, 0, "Save", output_saved);
LiquidScreen oSecondary_screen(oSave_line, back_line);

// This is the second menu.
LiquidMenu outputs_menu(lcd, pin6_screen, oSecondary_screen);


LiquidLine pinA4_line(0, 0, "Pin A4: ", pinA4_value);
LiquidLine pinA5_line(0, 1, "Pin A5: ", pinA5_value);
LiquidScreen pinsA4_A5_screen(pinA4_line, pinA5_line);

LiquidLine iSample_line(0, 0, "Sample: ", sample_period, "s");
LiquidLine iSave_line(0, 1, "Save", input_saved);
LiquidScreen iSecondary_screen(iSample_line, iSave_line, back_line);

// And this is the final third menu.
LiquidMenu inputs_menu(lcd, pinsA4_A5_screen, iSecondary_screen);

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

void increase_pin6() {
  if (pin6_level < 225) {
    pin6_level += 25;
  } else {
    pin6_level = 250;
  }
  analogWrite(pin6, pin6_level);
  output_saved = (char*)"  ";
}

void decrease_pin6() {
  if (pin6_level > 25) {
    pin6_level -= 25;
  } else {
    pin6_level = 0;
  }
  analogWrite(pin6, pin6_level);
  output_saved = (char*)"  ";
}

void save_input() {
  EEPROM.put(11, sample_period);
  input_saved = (char*)" *";
}

void save_output() {
  EEPROM.put(9, pin6_level);
  output_saved = (char*)" *";
}

void increase_samplePeriod() {
  if (sample_period < 10) {
    sample_period++;
    input_saved = (char*)"  ";
  }
}

void decrease_samplePeriod() {
  if (sample_period > 0) {
    sample_period--;
    input_saved = (char*)"  ";
  }
}

void setup() {
  Serial.begin(250000);

  pinMode(pin6, OUTPUT);

  // Reads the values recorded in the EEPROM
  EEPROM.get(9, pin6_level);
  EEPROM.get(11, sample_period);
  analogWrite(pin6, pin6_level);

  lcd.begin(16, 2);

  back_line.set_focusPosition(Position::LEFT);

  back_line.attach_function(1, go_back);
  back_line.attach_function(2, go_back);

  outputs_line.attach_function(1, goto_outputs_menu);
  outputs_line.attach_function(2, goto_outputs_menu);
  inputs_line.attach_function(1, goto_inputs_menu);
  inputs_line.attach_function(2, goto_inputs_menu);

  pin6_line.attach_function(increase, increase_pin6);
  pin6_line.attach_function(decrease, decrease_pin6);

  oSave_line.attach_function(1, save_output);
  oSave_line.attach_function(2, save_output);
  iSave_line.attach_function(1, save_input);
  iSave_line.attach_function(2, save_input);
  iSample_line.attach_function(increase, increase_samplePeriod);
  iSample_line.attach_function(decrease, decrease_samplePeriod);

  input_saved = (char*)" *";
  output_saved = (char*)" *";

  menu_system.update();
}

void loop() {
  buttonsCheck();

  static unsigned long lastMillis_sample = 0;
  if (millis() - lastMillis_sample > (sample_period * 1000)) {
    lastMillis_sample = millis();
    pinA4_value = analogRead(pinA4);
    pinA5_value = analogRead(pinA5);
    menu_system.update();
  }

}
