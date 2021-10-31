/*
 * LiquidMenu library - I2C_menu.ino
 * IMPORTANT: To configure the library for I2C connection define I2C
 * as "true" in the "LiquidMenu_config.h" file.
 *
 * This is the "hello_menu" example configured for I2C connection.
 *
 * The difference in using an I2C display library instead of the
 * official LiquidCrystal library is that void LiquidMenu::init()
 * method needs to be called in setup() after the I2C display library
 * is initialized. The other difference is that I2C needs to be defined
 * as "true" in the "LiquidMenu_config.h" file.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/I_I2C_menu/hello_menu.png
 * - PCF8574 module SCL to Arduino pin A5
 * - PCF8574 module SDA to Arduino pin A4
 * - PCF8574 module VCC to Arduino 5V
 * - PCF8574 module GND to Arduino GND
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

// The I2C LCD object
LiquidCrystal_I2C lcd(0x3F, 16, 2);

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
LiquidLine welcome_line2(1, 1, "Hello Menu I2C");

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

  // This is the I2C LCD object initialization.
  lcd.init();
  lcd.backlight();

  // Menu initialization.
  menu.init();

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
