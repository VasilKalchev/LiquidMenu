/*
 * LiquidMenu library - functions_menu.ino
 *
 * This example demonstrates how to attach functions to the
 * "lines" in the menu.
 *
 * To reduce memory footprint, returning value functions could be pass as line variable
 * instead of using a global variable definiton. Also, this allow to update required value
 * whithout the need of call another function before update().
 * 
 * All returning values are possible but functions can't have parameters, that is:
 *  uint8_t getUint8_t(), float getFloat(),...
 * 
 * However, you should use as quick as possible functions due to update performance. If you use heavy work functions,
 * you could get an unpleasent effect and get undesired delay between time to print variables.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/functions_menu/C_functions_menu.png
 * - LCD RS pin to Arduino pin 12
 * - LCD E pin to Arduino pin 11
 * - LCD D4 pin to Arduino pin 5
 * - LCD D5 pin to Arduino pin 4
 * - LCD D6 pin to Arduino pin 3
 * - LCD D7 pin to Arduino pin 2
 * - LCD R/W pin to ground
 * - LCD VSS pin to ground
 * - LCD VCC pin to 5V
 * - 10k ohm potentiometer: ends to 5V and ground, wiper to LCD V0
 * - 150 ohm resistor from 5V to LCD Anode
 * - LCD Cathode to ground
 * - ----
 * Created Ocboter 11, 2018
 * by José Manuel Pérez - https://github.com/jmpmscorp
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <LiquidCrystal.h>
#include <LiquidMenu.h>

/* Pin mapping for the display from DFRobot:
const byte LCD_RS = 8;
const byte LCD_E = 9;
const byte LCD_D4 = 4;
const byte LCD_D5 = 5;
const byte LCD_D6 = 6;
const byte LCD_D7 = 7;
*/

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



const byte ledPin = LED_BUILTIN;
/*
 * Variables used for periodic execution of code. The first one is the period
 * in milliseconds and the second one is the last time the code executed.
 */
unsigned int period_check = 1000;
unsigned long lastMs_check = 0;

unsigned int period_nextScreen = 5000;
unsigned long lastMs_nextScreen = 0;


const char onStr[] = "ON";
const char offStr[] = "OFF";

const char * readLed()
{
  return digitalRead(ledPin) == HIGH ? onStr : offStr;
}

unsigned int analogReadingA1()
{
  return analogRead(A1);
}

/*
 * LiquidLine objects represent a single line of text and/or variables
 * on the display. The first two parameters are the column and row from
 * which the line starts, the rest of the parameters are the text and/or
 * variables that will be printed on the display. They can be up to four.
 */
// Here the line is set to column 1, row 0 and will print the passed
// string and the passed variable.
LiquidLine welcome_line1(0, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
// Here the column is 3, the row is 1 and the string is "Hello Menu".
LiquidLine welcome_line2(0, 1, "Functions as Vars");

/*
 * LiquidScreen objects represent a single screen. A screen is made of
 * one or more LiquidLine objects. Up to four LiquidLine objects can
 * be inserted from here, but more can be added later in setup() using
 * welcome_screen.add_line(someLine_object);.
 */
// Here the LiquidLine objects are the two objects from above.
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

// Here there is not only a text string but also a changing integer variable.
LiquidLine analogReading_line(0, 0, "Analog: ", analogReadingA1);
LiquidLine led_line(0,1, "Led: ", readLed);
LiquidScreen secondary_screen(analogReading_line, led_line);

/*
 * The LiquidMenu object combines the LiquidScreen objects to form the
 * menu. Here it is only instantiated and the screens are added later
 * using menu.add_screen(someScreen_object);. This object is used to
 * control the menu, for example: menu.next_screen(), menu.switch_focus()...
 */
LiquidMenu menu(lcd);


void setup() {
  Serial.begin(250000);

  pinMode(A1, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, HIGH);

  lcd.begin(16, 2);

  // This is the method used to add a screen object to the menu.
  menu.add_screen(welcome_screen);
  menu.add_screen(secondary_screen);

  menu.update();
}

void loop() {
  // Periodic reading of the analog pin.
  if (millis() - lastMs_check > period_check) {
    lastMs_check = millis();
    
    if(digitalRead(ledPin) == HIGH){
      digitalWrite(ledPin, LOW);
    } 
    else{
      digitalWrite(ledPin, HIGH);
    }

    menu.updateIf(secondary_screen);
  }

  // Periodic switching to the next screen.
  if (millis() - lastMs_nextScreen > period_nextScreen) {
    lastMs_nextScreen = millis();
    menu.next_screen();
    
    LiquidScreen * ls = menu.get_current_screen();

    if( ls == &welcome_screen){
      Serial.println("Go to welcome_screen");
    }
    else if ( ls == &secondary_screen){
      Serial.println("Go to secondary_screen");
    }
  }
}