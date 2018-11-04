/*
 * LiquidMenu library - hello_menu.ino
 *
 * This is the get started example demonstrating how to create
 * a menu of two screens with dynamically changing information.
 *
 * The first screen shows some static text. The second screen
 * shows the reading of analog pin A1. To display a changing
 * variable on the LCD, simply put the variable in the LiquidLine
 * constructor. In this case the LiquidLine object is "analogReading_line"
 * and the variable is "analogReading". This line is on the second
 * screen. The value of the analog pin is read every second and if
 * it has changed the display is updated with the new value. The
 * menu cycles through its two screens every five seconds.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/A_hello_menu/hello_menu.png
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
//LCD R/W pin to ground
//10K potentiometer wiper to VO
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/*
 * Variable 'analogReading' is later configured to
 * be printed on the display. 'lastAnalogReading'
 * is used to check if the variable has changed.
 */
const byte analogPin = A1;
unsigned short analogReading = 0;
unsigned short lastAnalogReading = 0;

/*
 * Variables used for periodic execution of code. The first one is the period
 * in milliseconds and the second one is the last time the code executed.
 */
unsigned int period_check = 1000;
unsigned long lastMs_check = 0;

unsigned int period_nextScreen = 5000;
unsigned long lastMs_nextScreen = 0;

/*
 * LiquidLine objects represent a single line of text and/or variables
 * on the display. The first two parameters are the column and row from
 * which the line starts, the rest of the parameters are the text and/or
 * variables that will be printed on the display. They can be up to four.
 */
// Here the line is set to column 1, row 0 and will print the passed
// string and the passed variable.
LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
// Here the column is 3, the row is 1 and the string is "Hello Menu".
LiquidLine welcome_line2(3, 1, "Hello Menu");

/*
 * LiquidScreen objects represent a single screen. A screen is made of
 * one or more LiquidLine objects. Up to four LiquidLine objects can
 * be inserted from here, but more can be added later in setup() using
 * welcome_screen.add_line(someLine_object);.
 */
// Here the LiquidLine objects are the two objects from above.
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

// Here there is not only a text string but also a changing integer variable.
LiquidLine analogReading_line(0, 0, "Analog: ", analogReading);
LiquidScreen secondary_screen(analogReading_line);

/*
 * The LiquidMenu object combines the LiquidScreen objects to form the
 * menu. Here it is only instantiated and the screens are added later
 * using menu.add_screen(someScreen_object);. This object is used to
 * control the menu, for example: menu.next_screen(), menu.switch_focus()...
 */
LiquidMenu menu(lcd);


void setup() {
  Serial.begin(250000);

  pinMode(analogPin, INPUT);

  lcd.begin(16, 2);

  // This is the method used to add a screen object to the menu.
  menu.add_screen(welcome_screen);
  menu.add_screen(secondary_screen);
}

void loop() {
  // Periodic reading of the analog pin.
  if (millis() - lastMs_check > period_check) {
    lastMs_check = millis();
    analogReading = analogRead(analogPin);
    /*
     * Check if the analog value have changed
     * and update the display if it has.
     */
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
