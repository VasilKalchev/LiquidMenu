/*
 * LiquidMenu library - K_system_menu.ino
 *
 * Splitting a big menu into sub-menus with LiquidSystem.
 *
 * When a project grows, cycling through one long list of screens stops
 * being practical. LiquidSystem holds several LiquidMenu objects and
 * shows the screens of one of them at a time, so the screens can be
 * grouped: a main menu, an "Outputs" menu, an "Inputs" menu...
 *
 * LiquidSystem has the same interface as LiquidMenu, so once the system
 * is built every call is made on it instead of on the individual menus:
 *
 *   menu_system.next_screen();
 *   menu_system.change_menu(outputs_menu);   // the extra one
 *
 * change_menu() wrapped in a callback function is what turns a line
 * into a menu entry that opens a sub-menu, which is how "/Outputs" and
 * "/BACK" work below.
 *
 * The settings are kept in the EEPROM, so they survive a power cycle -
 * the "Save" lines write them and a "*" marks that what is on the
 * display is what is stored.
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
 * - Button (up) to Arduino pin 8 and ground
 * - Button (down) to Arduino pin 9 and ground
 * - Button (enter) to Arduino pin 10 and ground
 * - An LED (with a resistor) to Arduino pin 6 and ground
 * - 10k ohm potentiometer: ends to 5V and ground, wiper to Arduino pin A1
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
// LCD R/W pin to ground
// 10k potentiometer wiper to V0
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// The Button class is not a part of LiquidMenu (see "Button.h").
Button left(A0);
Button right(7);
Button up(8);
Button down(9);
Button enter(10);

enum FunctionTypes {
  increase = 1,
  decrease = 2,
};

// The addresses the settings are stored at.
const int EEPROM_LED_LEVEL = 0;
const int EEPROM_SAMPLE_PERIOD = 2;

const byte ledPin = 6;
byte ledLevel = 0;

const byte analogPin = A1;
unsigned short analogValue = 0;
// The sampling period in seconds, overwritten from the EEPROM below.
unsigned short samplePeriod = 2;

// " *" while the shown values are the stored ones, "  " otherwise.
char output_saved[3] = " *";
char input_saved[3] = " *";


// ----- The main menu -----
LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "System example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

// These two lines open the sub-menus.
LiquidLine outputs_line(0, 0, "/Outputs");
LiquidLine inputs_line(0, 1, "/Inputs");
LiquidScreen io_screen(outputs_line, inputs_line);

LiquidMenu main_menu(lcd, welcome_screen, io_screen);

// A LiquidLine object can be used in more than one menu.
LiquidLine back_line(11, 1, "/BACK");

// ----- The outputs menu -----
LiquidLine led_line(0, 0, "LED: ", ledLevel);
LiquidScreen led_screen(led_line);

LiquidLine outputSave_line(0, 0, "Save", output_saved);
LiquidScreen outputSave_screen(outputSave_line, back_line);

LiquidMenu outputs_menu(lcd, led_screen, outputSave_screen);

// ----- The inputs menu -----
LiquidLine analog_line(0, 0, "Analog: ", analogValue);
LiquidScreen analog_screen(analog_line);

LiquidLine samplePeriod_line(0, 0, "Sample: ", samplePeriod, "s");
LiquidLine inputSave_line(0, 1, "Save", input_saved);
LiquidScreen inputSave_screen(samplePeriod_line, inputSave_line, back_line);

LiquidMenu inputs_menu(lcd, analog_screen, inputSave_screen);

/*
 * The LiquidSystem object combines the menus. From here on the menus
 * are controlled through it.
 */
LiquidSystem menu_system(main_menu, outputs_menu, inputs_menu);


// ----- The callback functions -----
void go_back() {
  menu_system.change_menu(main_menu);
}

void goto_outputs_menu() {
  menu_system.change_menu(outputs_menu);
}

void goto_inputs_menu() {
  menu_system.change_menu(inputs_menu);
}

void increase_ledLevel() {
  if (ledLevel <= 230) {
    ledLevel += 25;
  }
  analogWrite(ledPin, ledLevel);
  strcpy(output_saved, "  ");
}

void decrease_ledLevel() {
  if (ledLevel >= 25) {
    ledLevel -= 25;
  }
  analogWrite(ledPin, ledLevel);
  strcpy(output_saved, "  ");
}

void save_output() {
  EEPROM.put(EEPROM_LED_LEVEL, ledLevel);
  strcpy(output_saved, " *");
}

void increase_samplePeriod() {
  if (samplePeriod < 10) {
    samplePeriod++;
    strcpy(input_saved, "  ");
  }
}

void decrease_samplePeriod() {
  if (samplePeriod > 1) {
    samplePeriod--;
    strcpy(input_saved, "  ");
  }
}

void save_input() {
  EEPROM.put(EEPROM_SAMPLE_PERIOD, samplePeriod);
  strcpy(input_saved, " *");
}

void checkButtons() {
  if (right.pressed()) {
    menu_system.next_screen();
  }
  if (left.pressed()) {
    menu_system.previous_screen();
  }
  if (enter.pressed()) {
    menu_system.switch_focus();
  }
  if (up.pressed()) {
    menu_system.call_function(increase);
  }
  if (down.pressed()) {
    menu_system.call_function(decrease);
  }
}

void setup() {
  Serial.begin(250000);

  pinMode(ledPin, OUTPUT);
  pinMode(analogPin, INPUT);

  lcd.begin(16, 2);

  // Restore the saved settings.
  EEPROM.get(EEPROM_LED_LEVEL, ledLevel);
  EEPROM.get(EEPROM_SAMPLE_PERIOD, samplePeriod);
  if (samplePeriod < 1 || samplePeriod > 10) { // a blank EEPROM
    samplePeriod = 2;
  }
  analogWrite(ledPin, ledLevel);

  back_line.set_focusPosition(Position::LEFT);

  // The same function under both numbers - either button opens the menu.
  outputs_line.attach_function(increase, goto_outputs_menu);
  outputs_line.attach_function(decrease, goto_outputs_menu);
  inputs_line.attach_function(increase, goto_inputs_menu);
  inputs_line.attach_function(decrease, goto_inputs_menu);
  back_line.attach_function(increase, go_back);
  back_line.attach_function(decrease, go_back);

  led_line.attach_function(increase, increase_ledLevel);
  led_line.attach_function(decrease, decrease_ledLevel);
  outputSave_line.attach_function(increase, save_output);
  outputSave_line.attach_function(decrease, save_output);

  samplePeriod_line.attach_function(increase, increase_samplePeriod);
  samplePeriod_line.attach_function(decrease, decrease_samplePeriod);
  inputSave_line.attach_function(increase, save_input);
  inputSave_line.attach_function(decrease, save_input);

  menu_system.update();
}

void loop() {
  checkButtons();

  static unsigned long lastMs_sample = 0;
  if (millis() - lastMs_sample > (unsigned long)samplePeriod * 1000) {
    lastMs_sample = millis();
    analogValue = analogRead(analogPin);
    menu_system.update();
  }
}
