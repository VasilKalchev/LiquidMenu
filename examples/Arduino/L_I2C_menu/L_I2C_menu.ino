/*
 * LiquidMenu library - L_I2C_menu.ino
 *
 * Using an I2C ("backpack") display instead of a parallel one.
 *
 * IMPORTANT: the library has to be told which display library it wraps,
 * because it is a compile time choice. Open "LiquidMenu_config.h" in the
 * library's "src" folder and switch the active block from the parallel
 * "LiquidCrystal" to the I2C one:
 *
 *   #define LIQUIDMENU_LIBRARY LiquidCrystal_I2C_LIBRARY
 *   #define DisplayClass LiquidCrystal_I2C
 *
 * Apart from that, only two things differ from the parallel examples:
 * the display object is constructed with its I2C address and size, and
 * menu.init() has to be called in setup() after the display library has
 * been initialized. Everything else - lines, screens, focus, callbacks -
 * is identical.
 *
 * This example uses the LiquidCrystal_I2C library:
 * https://github.com/johnrickman/LiquidCrystal_I2C
 * If your module shows nothing, its address is probably not 0x27 - the
 * other common one is 0x3F.
 *
 * Simulate it online:
 * Open this folder's "diagram.json" in Wokwi - see ../../README.md.
 *
 * The circuit:
 * - I2C module SDA to Arduino pin A4
 * - I2C module SCL to Arduino pin A5
 * - I2C module VCC to Arduino 5V
 * - I2C module GND to Arduino GND
 * - ----
 * - 10k ohm potentiometer: ends to 5V and ground, wiper to Arduino pin A1
 *
 * Created March 27, 2017
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <Wire.h>
// The I2C LCD library
#include <LiquidCrystal_I2C.h>
// The menu wrapper library
#include <LiquidMenu.h>

// The I2C address, the columns and the rows of the display.
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte analogPin = A1;

unsigned short analogReading = 0;
unsigned short lastAnalogReading = 0;

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "I2C example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine analogReading_line(0, 0, "Analog: ", analogReading);
LiquidScreen secondary_screen(analogReading_line);

LiquidMenu menu(lcd, welcome_screen, secondary_screen);


void setup() {
  Serial.begin(250000);

  pinMode(analogPin, INPUT);

  // The I2C display's own initialization.
  lcd.init();
  lcd.backlight();

  /*
   * Required with an I2C display: the display library is initialized
   * from setup() rather than from a constructor, so the menu has to be
   * initialized here too, after it.
   */
  menu.init();

  menu.update();
}

void loop() {
  static unsigned long lastMs_check = 0;
  if (millis() - lastMs_check > 1000) {
    lastMs_check = millis();
    analogReading = analogRead(analogPin);
    if (analogReading != lastAnalogReading) {
      lastAnalogReading = analogReading;
      menu.update();
    }
  }

  static unsigned long lastMs_nextScreen = 0;
  if (millis() - lastMs_nextScreen > 5000) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
