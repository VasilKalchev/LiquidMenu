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


LiquidLine welcome_line1(1, 0, "LiquidMenu ", VERSION);
LiquidLine welcome_line2(1, 1, "Serial example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine line21(4, 0, "Screen 2");
LiquidLine line22(1, 1, "next, previous");
LiquidScreen screen2(line21, line22);

LiquidLine line31(4, 0, "Screen 3");
LiquidLine line32(1, 1, "focus, position");
LiquidScreen screen3(line31, line32);

LiquidLine line41(4, 0, "Screen 4");
LiquidLine line42(1, 1, "update");
LiquidScreen screen4(line41, line42);

LiquidMenu menu(lcd, welcome_screen, screen2, screen3, screen4);

void setup() {
	Serial.begin(250000);
	lcd.begin(16, 2);

	menu.update();
}

void loop() {
	while (Serial.available() > 0) {
		String command = Serial.readStringUntil('\0');
		if (command == "next") {
			Serial.println(command);
			menu.next_screen();
		} else if (command == "previous") {
			Serial.println(command);
			menu.previous_screen();
		} else if (command == "focus") {
			Serial.println(command);
			menu.switch_focus(); // switches focus to the next line
			// focus is used with functions (see 'functions_menu' example)
		} else if (command == "focus position = left") {
			Serial.println(command);
			// this function sets the focus position
			menu.set_focusPosition(Position::LEFT);
		} else if (command == "focus position = right") {
			Serial.println(command);
			menu.set_focusPosition(Position::RIGHT);
		} else if (command == "update") {
			Serial.println(command);
		} else {
			Serial.print(command); Serial.println(" - unknown command");
		}
		menu.update();
	}
}
