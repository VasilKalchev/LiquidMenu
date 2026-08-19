/*
 * LiquidMenu library - B_I2C_menu.ino (ESP32)
 *
 * The usual ESP32 setup: an I2C ("backpack") display on two wires.
 *
 * IMPORTANT: the wrapped display library is a compile time choice. Open
 * "LiquidMenu_config.h" in the library's "src" folder and switch the
 * active block from the parallel "LiquidCrystal" to the I2C one:
 *
 *   #define LIQUIDMENU_LIBRARY LiquidCrystal_I2C_LIBRARY
 *   #define DisplayClass LiquidCrystal_I2C
 *
 * With an I2C display the display library is initialized from setup()
 * instead of from a constructor, so the menu has to be initialized there
 * as well, with menu.init(), after the display.
 *
 * On the ESP32 the default I2C pins are GPIO 21 (SDA) and GPIO 22 (SCL).
 * Any other pair can be used by starting the bus yourself before
 * lcd.init():
 *
 *   Wire.begin(SDA_PIN, SCL_PIN);
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
 * - I2C module SDA to ESP32 GPIO 21
 * - I2C module SCL to ESP32 GPIO 22
 * - I2C module VCC to ESP32 5V (VIN)
 * - I2C module GND to ESP32 GND
 * - ----
 * - 10k ohm potentiometer: ends to 3.3V and ground, wiper to GPIO 34
 *
 * Created August 20, 2026
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

// The default I2C pins of the ESP32.
const byte I2C_SDA = 21;
const byte I2C_SCL = 22;

// An ADC capable, input only GPIO.
const byte analogPin = 34;

unsigned short analogReading = 0;
unsigned short lastAnalogReading = 0;

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "ESP32 over I2C");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine analogReading_line(0, 0, "Analog: ", analogReading);
LiquidScreen secondary_screen(analogReading_line);

LiquidMenu menu(lcd, welcome_screen, secondary_screen);


void setup() {
  Serial.begin(115200);

  pinMode(analogPin, INPUT);

  // Starting the bus explicitly makes the used pins visible - and lets
  // them be changed.
  Wire.begin(I2C_SDA, I2C_SCL);

  lcd.init();
  lcd.backlight();

  // Required with an I2C display, after the display's own init.
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
