/*
 * LiquidMenu library - A_hello_menu.ino
 *
 * The "get started" example: a menu of two screens, one of them showing
 * a variable that changes while the sketch runs.
 *
 * It introduces the three classes the library is built on:
 *  - LiquidLine  - one line of text and/or variables on the display,
 *  - LiquidScreen - the lines that are shown together at one time,
 *  - LiquidMenu  - the screens, plus the methods used to navigate them.
 *
 * A variable put in a LiquidLine object is printed with its current
 * value every time the display is refreshed - the line keeps a reference
 * to it, so there is nothing to re-assign. Call menu.update() when the
 * value has changed and the new value appears on the display.
 *
 * Here the second screen shows the reading of an analog pin, sampled
 * once a second, and the menu cycles through its two screens every five
 * seconds.
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
 * - some analog input to Arduino pin A1 (unconnected also works)
 *
 * Created July 24, 2016
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

// The LCD library
#include <LiquidCrystal.h>
// The menu wrapper library
#include <LiquidMenu.h>

// Pin mapping for the display:
const byte LCD_RS = 12;
const byte LCD_E = 11;
const byte LCD_D4 = 5;
const byte LCD_D5 = 4;
const byte LCD_D6 = 3;
const byte LCD_D7 = 2;
// LCD R/W pin to ground
// 10k potentiometer wiper to V0
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

const byte analogPin = A1;

/*
 * The variable 'analogReading' is configured below to be printed on the
 * display. 'lastAnalogReading' is used to check whether it has changed,
 * because the display only needs to be refreshed when it has.
 */
unsigned short analogReading = 0;
unsigned short lastAnalogReading = 0;

// Variables used for the periodic execution of code: the period in
// milliseconds and the last time the code ran.
const unsigned int period_check = 1000;
unsigned long lastMs_check = 0;

const unsigned int period_nextScreen = 5000;
unsigned long lastMs_nextScreen = 0;

/*
 * LiquidLine objects represent a single line of text and/or variables on
 * the display. The first two parameters are the column and the row the
 * line starts at, the rest are the text and/or the variables that will
 * be printed. They can be up to four.
 */
// This line starts at column 1 of row 0 and prints a string
// literal followed by the library's version.
LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
// This one starts at column 3 of row 1 and prints just a string.
LiquidLine welcome_line2(3, 1, "Hello Menu");

/*
 * A LiquidScreen object represents what is shown on the display at one
 * time. Up to four LiquidLine objects can be passed to the constructor,
 * more can be added later with welcome_screen.add_line(some_line).
 */
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

// This line mixes a string literal with a variable. Every refresh of the
// display prints the value 'analogReading' holds at that moment.
LiquidLine analogReading_line(0, 0, "Analog: ", analogReading);
LiquidScreen secondary_screen(analogReading_line);

/*
 * The LiquidMenu object combines the screens into a menu. Here it is
 * only given the display object and the screens are added later with
 * menu.add_screen(some_screen). This object is what the menu is
 * controlled through: menu.next_screen(), menu.switch_focus()...
 */
LiquidMenu menu(lcd);


void setup() {
  Serial.begin(250000);

  pinMode(analogPin, INPUT);

  lcd.begin(16, 2);

  // The screens are shown in the order they are added.
  menu.add_screen(welcome_screen);
  menu.add_screen(secondary_screen);

  // Print the first screen.
  menu.update();
}

void loop() {
  // Periodic reading of the analog pin.
  if (millis() - lastMs_check > period_check) {
    lastMs_check = millis();
    analogReading = analogRead(analogPin);

    // Refresh the display only if the value has actually changed.
    if (analogReading != lastAnalogReading) {
      lastAnalogReading = analogReading;
      menu.update();
    }
  }

  // Periodic switching to the next screen.
  if (millis() - lastMs_nextScreen > period_nextScreen) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
