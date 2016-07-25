#include <LiquidCrystal.h>
#include <LiquidMenu.h>

// Pin mapping for the display
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
 * Variables 'analogReading1' and 'analogReading2' are later configured to
 * be printed on the display. 'lastAnalogReading1' and 'lastAnalogReading2'
 * are used to check if the variable has changed.
 */
const byte analogPin1 = A1;
unsigned short analogReading1 = 0;
unsigned short lastAnalogReading1 = 0;

const byte analogPin2 = A2;
unsigned short analogReading2 = 0;
unsigned short lastAnalogReading2 = 0;

/*
 * Variables used for periodic execution of code. The first one is the period
 * in milliseconds and the second one is the last time the code executed.
 */
unsigned int period_check = 500;
unsigned long lastMs_check = 0;

unsigned int period_next = 5000;
unsigned long lastMs_next = 0;

/*
 * LiquidLine objects represent a single line of text and/or variables
 * on the display. The first two parameters are the column and row from
 * which the line starts, the rest of the parameters are the text and/or
 * variables that will be printed on the display. They can be up to four.
 */
// Here the line is set to column 1, row 0 and will print the passed
// string and the passed variable.
LiquidLine welcome_line1(1, 0, "LiquidMenu ", VERSION);
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
LiquidLine analog1_line(0, 0, "Analog 1: ", analogReading1);
LiquidLine analog2_line(0, 1, "Analog 2: ", analogReading2);
LiquidScreen secondary_screen(analog1_line, analog2_line);

/*
 * The LiquidMenu object combines the LiquidScreen objects to form the
 * menu. Here it is only instantiated and the screens are added later
 * using menu.add_screen(someScreen_object);. This object is used to
 * control the menu, for example: menu.next_screen(), menu.switch_focus()...
 */
LiquidMenu menu(lcd);


void setup() {
	Serial.begin(250000);

	pinMode(analogPin1, INPUT);
	pinMode(analogPin2, INPUT);

	lcd.begin(16, 2);

	// This is the method used to add a screen object to the menu.
	menu.add_screen(welcome_screen);
	menu.add_screen(secondary_screen);
}

void loop() {
	/*
	 * Check if the analog values have changed
	 * and update the display if they have.
	 */
	if (analogReading1 != lastAnalogReading1) {
		lastAnalogReading1 = analogReading1;
		menu.update();
	}
	if (analogReading2 != lastAnalogReading2) {
		lastAnalogReading2 = analogReading2;
		menu.update();
	}

	// Periodic reading of the analog pins.
	if (millis() - lastMs_check > period_check) {
		lastMs_check = millis();
		analogReading1 = analogRead(analogPin1);
		analogReading2 = analogRead(analogPin2);
	}

	// Periodic switching to the next screen.
	if (millis() - lastMs_next > period_next) {
		lastMs_next = millis();
		menu.next_screen();
	}
}
