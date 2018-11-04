/*
 * LiquidMenu library - serial_menu.ino
 *
 * This example uses the serial communication to execute commands.
 *
 * The available commands will be printed on the serial monitor.
 * This example uses attached callback functions which are explained
 * in the functions_menu.ino example.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/B_serial_menu/serial_menu.png
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
 *
 * Created July 24, 2016
 * by Vasil Kalchev
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
//LCD R/W pin to ground
//10K potentiometer wiper to VO
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "Serial example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine line21(4, 0, "Screen 2");
LiquidLine type_line(4, 1, "type...");
LiquidScreen screen2(line21, type_line);

LiquidLine line31(4, 0, "Screen 3");
LiquidScreen screen3(line31, type_line);

LiquidLine line41(4, 0, "Screen 4");
LiquidScreen screen4(line41, type_line);

LiquidMenu menu(lcd, welcome_screen, screen2, screen3, screen4);

/*
 * This function will be attached to one or more LiquidLine
 * objects and will be called when the line is focused and
 * bool LiquidMenu::call_function(byte number) is called with
 * number - the number specified when attaching the function.
 * More on this in the function_menu.ino example.
*/
void callback_function() {
  Serial.println(F("You called the callback function."));
}

void setup() {
  Serial.begin(250000);

  lcd.begin(16, 2);

  // Here we attach the function defined earlier to four LiquidLine objects.
  line21.attach_function(1, callback_function);
  line31.attach_function(1, callback_function);
  line41.attach_function(1, callback_function);
  type_line.attach_function(1, callback_function);

  menu.update();

  Serial.print(F("LiquidMenu ")); Serial.print(LIQUIDMENU_VERSION); Serial.println(F(" - Serial example:"));
  Serial.println(F("To navigate the menu type 'menu.next_screen()' or 'menu.previous_screen()'."));
  Serial.println(F("To switch focus type 'menu.switch_focus()'."));
  Serial.println(F("To call the callback function type 'menu.call_function(1)'."));
  Serial.println(F("To adjust the focus position type 'menu.set_focusPosition(Position::LEFT)'"));
  Serial.println(F("  or 'menu.set_focusPosition(Position::RIGHT)'."));
  Serial.println(F("To update the display type 'menu.update()'.")); Serial.println();
}

void loop() {
  // This handles the serial input.
  while (Serial.available() > 0) {
    String command = Serial.readStringUntil('\0');
    if (command == "menu.next_screen()") {
      Serial.println(command);
      menu.next_screen();
    } else if (command == "menu.previous_screen()") {
      Serial.println(command);
      menu.previous_screen();
    } else if (command == "menu.switch_focus()") {
      Serial.println(command);
      menu.switch_focus(); // switches focus to the next line
      // focus is used with functions (see 'functions_menu' example)
    } else if (command == "menu.call_function(1)") {
      Serial.println(command);
      // The attached function is called.
      menu.call_function(1);
    } else if (command == "menu.set_focusPosition(Position::LEFT)") {
      Serial.println(command);
      // this function sets the focus position
      menu.set_focusPosition(Position::LEFT);
    } else if (command == "menu.set_focusPosition(Position::RIGHT)") {
      Serial.println(command);
      menu.set_focusPosition(Position::RIGHT);
    } else if (command == "menu.update()") {
      Serial.println(command);
      menu.update();
    } else {
      Serial.print(command); Serial.println(F(" - unknown command"));
    }
    menu.update();
  }
}
