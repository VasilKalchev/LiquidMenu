/*
 * LiquidMenu library - D_system_menu.ino (ESP32)
 *
 * Sub-menus with LiquidSystem, with the settings kept in the ESP32's
 * NVS flash storage.
 *
 * LiquidSystem holds several LiquidMenu objects and shows the screens of
 * one of them at a time, which is how a menu is split into sections: a
 * main menu, an "Outputs" menu, an "Inputs" menu. It has the same
 * interface as LiquidMenu plus change_menu(), and wrapping change_menu()
 * in a callback function is what turns a line into an entry that opens a
 * sub-menu ("/Outputs", "/BACK").
 *
 * The ESP32 has no EEPROM. The Arduino core emulates one, but the native
 * way to keep a handful of settings is the Preferences library, which
 * stores key/value pairs in the NVS partition of the flash - that is
 * what this example uses:
 *
 *   prefs.begin("liquidmenu", false);   // namespace, read/write
 *   prefs.putUChar("led", ledLevel);
 *   ledLevel = prefs.getUChar("led", 0);   // 0 if never stored
 *
 * The display is on I2C - set the library to the I2C display library in
 * "LiquidMenu_config.h", see the "B_I2C_menu" example.
 *
 * Simulate it online:
 * Open this folder's "diagram.json" in Wokwi - see ../../README.md.
 *
 * The circuit:
 * - I2C module SDA to ESP32 GPIO 21
 * - I2C module SCL to ESP32 GPIO 22
 * - I2C module VCC to ESP32 5V (VIN)
 * - I2C module GND to ESP32 GND
 * - ----
 * - Button (left) to ESP32 GPIO 32 and ground
 * - Button (right) to ESP32 GPIO 33 and ground
 * - Button (up) to ESP32 GPIO 25 and ground
 * - Button (down) to ESP32 GPIO 26 and ground
 * - Button (enter) to ESP32 GPIO 27 and ground
 * - An LED (with a resistor) to ESP32 GPIO 4 and ground
 * - 10k ohm potentiometer: ends to 3.3V and ground, wiper to GPIO 35
 *
 * Created August 20, 2026
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>
#include <Preferences.h>
#include "Button.h"

// The I2C address, the columns and the rows of the display.
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte I2C_SDA = 21;
const byte I2C_SCL = 22;

// The Button class is not a part of LiquidMenu (see "Button.h").
Button left(32);
Button right(33);
Button up(25);
Button down(26);
Button enter(27);

// The non-volatile storage.
Preferences prefs;

enum FunctionTypes {
  increase = 1,
  decrease = 2,
};

const byte ledPin = 4;
byte ledLevel = 0;

// An ADC capable, input only GPIO.
const byte analogPin = 35;
unsigned short analogValue = 0;
unsigned short samplePeriod = 2;  // in seconds

// " *" while the shown values are the stored ones, "  " otherwise.
char output_saved[3] = " *";
char input_saved[3] = " *";


// ----- The main menu -----
LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "ESP32 system");
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

// The menus combined into a menu system.
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
  prefs.putUChar("led", ledLevel);
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
  prefs.putUShort("sample", samplePeriod);
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
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(analogPin, INPUT);

  Wire.begin(I2C_SDA, I2C_SCL);

  lcd.init();
  lcd.backlight();

  /*
   * Required with an I2C display, after the display's own init.
   * LiquidSystem has no init() of its own - the call only creates the
   * focus indicator characters on the display, so calling it on any one
   * of the menus covers the whole system.
   */
  main_menu.init();

  // Restore the saved settings - the second argument is the value to
  // use when the key has never been stored.
  prefs.begin("liquidmenu", false);
  ledLevel = prefs.getUChar("led", 0);
  samplePeriod = prefs.getUShort("sample", 2);
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
