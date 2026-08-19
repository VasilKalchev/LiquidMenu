/*
 * LiquidMenu library - I_progmem_menu.ino
 *
 * Printing text that is kept in flash memory instead of RAM.
 *
 * An ATmega328 (Uno, Nano) has 2KB of RAM but 32KB of flash, and every
 * string literal in a sketch takes up RAM. Long menu texts can be kept
 * in flash instead by declaring them "const ... PROGMEM" and telling the
 * line which of its variables lives there:
 *
 *   const char text[] PROGMEM = "Stored in flash";
 *   LiquidLine line(0, 0, text);
 *   ...
 *   line.set_asProgmem(1);   // 1 - the first variable of the line
 *
 * Without set_asProgmem() the line would print whatever RAM happens to
 * be at that address, so the call is not optional.
 *
 * PROGMEM is an AVR feature. On architectures with a unified address
 * space (ESP32, RP2040, STM32...) the macro exists for compatibility but
 * does nothing, and this whole technique is unnecessary.
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
 *
 * Created July 24, 2016
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <avr/pgmspace.h> // The PROGMEM macros

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

// The texts that are kept in flash memory. Each one is 16 characters,
// the width of the display.
const char help1[] PROGMEM = "Long menu texts ";
const char help2[] PROGMEM = "eat up the RAM. ";
const char help3[] PROGMEM = "PROGMEM keeps   ";
const char help4[] PROGMEM = "them in flash.  ";

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(0, 1, "PROGMEM example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine help_line1(0, 0, help1);
LiquidLine help_line2(0, 1, help2);
LiquidScreen help_screen1(help_line1, help_line2);

LiquidLine help_line3(0, 0, help3);
LiquidLine help_line4(0, 1, help4);
LiquidScreen help_screen2(help_line3, help_line4);

LiquidMenu menu(lcd, welcome_screen, help_screen1, help_screen2);


void setup() {
  Serial.begin(250000);

  lcd.begin(16, 2);

  /*
   * Marks the flash stored variables. The parameter is the consecutive
   * number of the variable for that line - each of these lines holds a
   * single variable, so it is always 1.
   */
  help_line1.set_asProgmem(1);
  help_line2.set_asProgmem(1);
  help_line3.set_asProgmem(1);
  help_line4.set_asProgmem(1);

  menu.update();
}

void loop() {
  static unsigned long lastMs_nextScreen = 0;
  if (millis() - lastMs_nextScreen > 3000) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
