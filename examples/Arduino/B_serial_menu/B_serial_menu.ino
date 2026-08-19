/*
 * LiquidMenu library - B_serial_menu.ino
 *
 * Drives the menu from the Serial Monitor instead of from buttons.
 *
 * This is the example to try first if you don't have the buttons wired
 * up yet - every navigation method of the library is bound to a typed
 * command, so you can watch what each one does on the display:
 *
 *   n  - next_screen()      p  - previous_screen()
 *   f  - switch_focus()     c  - call_function(1)
 *   l  - focus indicator on the left
 *   r  - focus indicator on the right
 *   u  - update()
 *
 * "Focus" is the library's word for the selected line. Only a focused
 * line can have one of its callback functions called, and only a line
 * with a function attached to it can be focused - which is why the blank
 * "callback_function" below is attached to the lines here. Callbacks are
 * covered properly in the "C_buttons_menu" example.
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
// LCD R/W pin to ground
// 10k potentiometer wiper to V0
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "Serial example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine screen2_line1(4, 0, "Screen 2");
LiquidLine screen3_line1(4, 0, "Screen 3");
LiquidLine screen4_line1(4, 0, "Screen 4");

// A LiquidLine object can be used on more than one screen.
LiquidLine type_line(4, 1, "type...");

LiquidScreen screen2(screen2_line1, type_line);
LiquidScreen screen3(screen3_line1, type_line);
LiquidScreen screen4(screen4_line1, type_line);

LiquidMenu menu(lcd, welcome_screen, screen2, screen3, screen4);

/*
 * A callback function has no parameters and returns nothing. This one is
 * attached to the lines below under the number 1, so it is called for
 * whichever line is focused when menu.call_function(1) runs.
 */
void callback_function() {
  Serial.println(F("The callback function of the focused line was called."));
}

void print_commands() {
  Serial.print(F("LiquidMenu "));
  Serial.print(LIQUIDMENU_VERSION);
  Serial.println(F(" - serial example. Available commands:"));
  Serial.println(F("  n - menu.next_screen()"));
  Serial.println(F("  p - menu.previous_screen()"));
  Serial.println(F("  f - menu.switch_focus()"));
  Serial.println(F("  c - menu.call_function(1)"));
  Serial.println(F("  l - menu.set_focusPosition(Position::LEFT)"));
  Serial.println(F("  r - menu.set_focusPosition(Position::RIGHT)"));
  Serial.println(F("  u - menu.update()"));
  Serial.println();
}

void setup() {
  Serial.begin(250000);

  lcd.begin(16, 2);

  // Attaching a function to a line is what makes the line focusable.
  screen2_line1.attach_function(1, callback_function);
  screen3_line1.attach_function(1, callback_function);
  screen4_line1.attach_function(1, callback_function);
  type_line.attach_function(1, callback_function);

  menu.update();

  print_commands();
}

void loop() {
  while (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case 'n':
        Serial.println(F("menu.next_screen()"));
        menu.next_screen();
        break;
      case 'p':
        Serial.println(F("menu.previous_screen()"));
        menu.previous_screen();
        break;
      case 'f':
        Serial.println(F("menu.switch_focus()"));
        // Moves the focus to the next focusable line of this screen.
        menu.switch_focus();
        break;
      case 'c':
        Serial.println(F("menu.call_function(1)"));
        // Calls function 1 of the currently focused line, if it has one.
        menu.call_function(1);
        break;
      case 'l':
        Serial.println(F("menu.set_focusPosition(Position::LEFT)"));
        menu.set_focusPosition(Position::LEFT);
        menu.update();
        break;
      case 'r':
        Serial.println(F("menu.set_focusPosition(Position::RIGHT)"));
        menu.set_focusPosition(Position::RIGHT);
        menu.update();
        break;
      case 'u':
        Serial.println(F("menu.update()"));
        menu.update();
        break;
      case '\r':
      case '\n':
        break;
      default:
        Serial.print(command);
        Serial.println(F(" - unknown command"));
        print_commands();
        break;
    }
  }
}
