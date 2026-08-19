/*
 * LiquidMenu library - H_getters_menu.ino
 *
 * Printing the return value of a function instead of a variable.
 *
 * Everywhere a LiquidLine takes a variable it can take a "getter"
 * function instead - a function with no parameters that returns the
 * value to print:
 *
 *   int read_pot() { return analogRead(potPin); }
 *   LiquidLine value_line(0, 0, "Value: ", read_pot);
 *
 * Note that the function is passed by name, without the parentheses.
 * The line then calls it on every refresh, which means there is no
 * mirror variable to keep up to date in loop() - often a smaller sketch
 * and a smaller RAM footprint.
 *
 * The return type can be any of the types the library can print,
 * including "const char*" for text, as the second getter here shows.
 * Keep the getters quick: they run on every refresh of the display.
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
 * - 10k ohm potentiometer: ends to 5V and ground, wiper to Arduino pin A1
 *
 * Created October 11, 2018
 * by Jose Manuel Perez - https://github.com/jmpmscorp
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <LiquidCrystal.h>
#include <LiquidMenu.h>

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

const byte potPin = A1;

// The first getter function - it returns the potentiometer's reading.
int read_pot() {
  return analogRead(potPin);
}

// The strings used by the second getter function.
const char potMinStr[] = "minimum";
const char potLowStr[] = "low    ";
const char potHighStr[] = "high   ";
const char potMaxStr[] = "maximum";

// A getter can also return a pointer to a string.
const char* get_potPosition() {
  int potValue = read_pot();
  if (potValue < 10) {
    return potMinStr;
  } else if (potValue < 512) {
    return potLowStr;
  } else if (potValue < 1000) {
    return potHighStr;
  } else {
    return potMaxStr;
  }
}

LiquidLine welcome_line1(0, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(0, 1, "Getters example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

// The getters are passed by name, without the parentheses.
LiquidLine potValue_line(0, 0, "Value: ", read_pot);
LiquidLine potPosition_line(0, 1, "Knob:  ", get_potPosition);
LiquidScreen pot_screen(potValue_line, potPosition_line);

LiquidMenu menu(lcd, welcome_screen, pot_screen);

void setup() {
  Serial.begin(250000);

  pinMode(potPin, INPUT);

  lcd.begin(16, 2);

  // Show the welcome screen as a splash screen, then move on.
  menu.update();
  delay(2000);
  menu.change_screen(&pot_screen);
}

void loop() {
  /*
   * The getters are called by the refresh, so keeping the display up to
   * date takes nothing more than this - there is no variable to update.
   */
  static unsigned long lastMs_update = 0;
  if (millis() - lastMs_update > 500) {
    lastMs_update = millis();
    menu.softUpdate();
  }
}
