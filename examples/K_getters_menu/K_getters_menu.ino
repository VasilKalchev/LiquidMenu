/*
 * LiquidMenu library - getters_menu.ino
 *
 * This example demonastrates how to use getter functions instead of
 * variables in "LiquidScreen" objects.
 *
 * It's possible to replace the variables in the "LiquidScreen"
 * objects with getter functions in order to simplify the code in the
 * main loop and reduce memory footprint.
 * The getter functions can be of any return type, but they must have
 * no parameters (e.g. bool getButtonState(), int getPosition()...).
 * 
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/K_getters_menu/getters_menu.png
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
 * - some analog input to Arduino pin A1 (unconnected also works)
 *
 * Created Ocboter 11, 2018
 * by José Manuel Pérez - https://github.com/jmpmscorp
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <LiquidCrystal.h>
#include <LiquidMenu.h>

const byte LCD_RS = 12;
const byte LCD_E = 11;
const byte LCD_D4 = 5;
const byte LCD_D5 = 4;
const byte LCD_D6 = 3;
const byte LCD_D7 = 2;
//LCD R/W pin to ground
//10K potentiometer wiper to VO
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

const byte potPin = A1;

unsigned long updatePeriod = 500;
unsigned long updateLastMs = 0;

// This is the first getter function, it reads the potentiometer value.
int readPot() {
  return analogRead(potPin);
}

// This strings will be used by the second getter function.
const char potMinStr[] = "minimum";
const char potLowStr[] = "low";
const char potHighStr[] = "high";
const char potMaxStr[] = "maximum";

// This getter function demonstrates how to return a pointer to a string.
const char* getPotPosition() {
  unsigned int potValue = readPot();
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
LiquidLine welcome_line2(0, 1, "Getters menu");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine potValue_line(0, 0, "Value: ", readPot);
LiquidLine potPosition_line(0, 1, getPotPosition);
LiquidScreen pot_screen(potValue_line, potPosition_line);

LiquidMenu menu(lcd);

void setup() {
  Serial.begin(250000);
  Serial.println("Getters menu");

  pinMode(potPin, INPUT);

  lcd.begin(16, 2);

  menu.add_screen(welcome_screen);
  menu.add_screen(pot_screen);

  // Display the welcome screen as a splash screen.
  menu.update();
  delay(1000);
  if (menu.get_currentScreen() == &welcome_screen) {
    menu.change_screen(&pot_screen);
    menu.update();
  }
}

void loop() {
  if (millis() - updateLastMs > updatePeriod) {
    updateLastMs = millis();

    menu.update();
  }
}
