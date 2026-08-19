/*
 * LiquidMenu library - A_hello_menu.ino (ESP32)
 *
 * The "get started" example for the ESP32: a menu of two screens, the
 * second one showing a variable that changes while the sketch runs.
 *
 * The library itself is the same on every board - LiquidLine holds the
 * text and the variables, LiquidScreen groups the lines that are shown
 * together and LiquidMenu holds the screens and the navigation. What is
 * different on an ESP32 is the board around it:
 *
 *  - the pin numbers are GPIO numbers, and not all of them are usable:
 *    GPIO 6-11 are taken by the flash, GPIO 34-39 are input only, and
 *    GPIO 0, 2, 12 and 15 are strapping pins that decide how the chip
 *    boots, so they are avoided here,
 *  - there is no "A1" style alias for the analog pins - an ADC capable
 *    GPIO is used by its number (GPIO 34 below),
 *  - analogRead() returns 0..4095 by default (12 bit) instead of
 *    0..1023,
 *  - the ESP32 runs on 3.3V. Most HD44780 modules are happy with 3.3V
 *    logic while powered from 5V (as wired here); if yours shows nothing
 *    but a row of blocks, power it from 3.3V or use a level shifter.
 *
 * Wiring six data lines to a display is unusual on an ESP32 - the I2C
 * variant in "B_I2C_menu" is the more common choice.
 *
 * Simulate it online:
 * Open this folder's "diagram.json" in Wokwi - see ../../README.md.
 *
 * The circuit:
 * - LCD RS pin to ESP32 GPIO 13
 * - LCD E pin to ESP32 GPIO 14
 * - LCD D4 pin to ESP32 GPIO 27
 * - LCD D5 pin to ESP32 GPIO 26
 * - LCD D6 pin to ESP32 GPIO 25
 * - LCD D7 pin to ESP32 GPIO 33
 * - LCD R/W pin to ground
 * - LCD VSS pin to ground
 * - LCD VDD pin to 5V
 * - 10k ohm potentiometer: ends to 5V and ground, wiper to LCD V0
 * - LCD Anode to 5V (through a 150 ohm resistor), Cathode to ground
 * - ----
 * - 10k ohm potentiometer: ends to 3.3V and ground, wiper to GPIO 34
 *
 * Created August 20, 2026
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

// The LCD library
#include <LiquidCrystal.h>
// The menu wrapper library
#include <LiquidMenu.h>

// Pin mapping for the display (GPIO numbers):
const byte LCD_RS = 13;
const byte LCD_E = 14;
const byte LCD_D4 = 27;
const byte LCD_D5 = 26;
const byte LCD_D6 = 25;
const byte LCD_D7 = 33;
// LCD R/W pin to ground
// 10k potentiometer wiper to V0
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// An ADC capable, input only GPIO.
const byte analogPin = 34;

unsigned short analogReading = 0;
unsigned short lastAnalogReading = 0;

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(3, 1, "Hello ESP32");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine analogReading_line(0, 0, "Analog: ", analogReading);
LiquidScreen secondary_screen(analogReading_line);

LiquidMenu menu(lcd, welcome_screen, secondary_screen);


void setup() {
  Serial.begin(115200);

  pinMode(analogPin, INPUT);

  lcd.begin(16, 2);

  menu.update();
}

void loop() {
  // Periodic reading of the analog pin.
  static unsigned long lastMs_check = 0;
  if (millis() - lastMs_check > 1000) {
    lastMs_check = millis();
    analogReading = analogRead(analogPin);

    // Refresh the display only if the value has actually changed.
    if (analogReading != lastAnalogReading) {
      lastAnalogReading = analogReading;
      menu.update();
    }
  }

  // Periodic switching to the next screen.
  static unsigned long lastMs_nextScreen = 0;
  if (millis() - lastMs_nextScreen > 5000) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
