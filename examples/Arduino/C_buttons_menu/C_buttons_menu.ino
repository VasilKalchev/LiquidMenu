/*
 * LiquidMenu library - C_buttons_menu.ino
 *
 * The complete "interactive menu" pattern: five buttons that navigate the
 * screens, move the focus and call functions that change the hardware.
 *
 * A function attached to a line is called a callback. It takes no
 * parameters and returns nothing, and it is attached with:
 *
 *   line.attach_function(number, function);
 *
 * The "number" is what makes multiple functions per line possible. Only
 * the focused line's function is called, so functions that do the same
 * kind of thing should share a number across the different lines: here
 * every "make it bigger" function is attached under 'increase' and every
 * "make it smaller" one under 'decrease'. The UP button then simply calls
 * menu.call_function(increase) and the right thing happens for whichever
 * line is focused.
 *
 * Attaching a function is also what makes a line focusable - a line with
 * no functions is skipped by menu.switch_focus().
 *
 * Text that changes at runtime has to live in a char array (a string
 * literal is fixed at compile time), which is what 'blinking_text' is
 * for - the line prints whatever the array holds at the moment.
 *
 * Try it: press RIGHT until the "LED" screen, press ENTER to focus the
 * line, then UP/DOWN to dim the LED.
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
 * - Button (left) to Arduino pin A0 and ground
 * - Button (right) to Arduino pin 7 and ground
 * - Button (up) to Arduino pin 8 and ground
 * - Button (down) to Arduino pin 9 and ground
 * - Button (enter) to Arduino pin 10 and ground
 * - An LED (with a resistor) to Arduino pin 6 and ground
 *
 * Created July 24, 2016
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include "Button.h"

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

/*
 * The Button class is not a part of LiquidMenu, it is bundled with the
 * example (see "Button.h"). The buttons are wired to ground and use the
 * internal pull-up resistors.
 */
Button left(A0);
Button right(7);
Button up(8);
Button down(9);
Button enter(10);

/*
 * Naming the callback numbers keeps the calls readable. Everything that
 * increases a value is attached as 'increase', everything that decreases
 * one as 'decrease'.
 */
enum FunctionTypes {
  increase = 1,
  decrease = 2,
};

// The PWM controlled LED and its current level.
const byte ledPin = 6;
byte ledLevel = 0;

// The blinking "preference" and the text that shows its state. Changing
// text needs a char array, not a string literal.
bool isBlinking = false;
char blinking_text[4] = "OFF";
unsigned int blinkPeriod = 500;


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "Buttons example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine led_title_line(6, 0, "LED");
LiquidLine led_line(4, 1, "Level: ", ledLevel);
LiquidScreen led_screen(led_title_line, led_line);

LiquidLine blink_line(0, 0, "Blink - ", blinking_text);
LiquidLine blinkPeriod_line(0, 1, "Period: ", blinkPeriod, "ms");
LiquidScreen blink_screen(blink_line, blinkPeriod_line);

LiquidMenu menu(lcd, welcome_screen, led_screen, blink_screen);


// The callback functions.
void increase_ledLevel() {
  if (ledLevel <= 230) {
    ledLevel += 25;
  }
  analogWrite(ledPin, ledLevel);
}

void decrease_ledLevel() {
  if (ledLevel >= 25) {
    ledLevel -= 25;
  }
  analogWrite(ledPin, ledLevel);
}

// Attached under both numbers - UP and DOWN both toggle the blinking.
void toggle_blinking() {
  isBlinking = !isBlinking;
  strcpy(blinking_text, isBlinking ? "ON" : "OFF");
  ledLevel = 0;
  analogWrite(ledPin, ledLevel);
}

void increase_blinkPeriod() {
  if (blinkPeriod <= 2900) {
    blinkPeriod += 100;
  }
}

void decrease_blinkPeriod() {
  if (blinkPeriod >= 200) {
    blinkPeriod -= 100;
  }
}

void checkButtons() {
  if (right.pressed()) {
    menu.next_screen();
  }
  if (left.pressed()) {
    menu.previous_screen();
  }
  if (enter.pressed()) {
    // Moves the focus to the next focusable line of this screen.
    menu.switch_focus();
  }
  if (up.pressed()) {
    // Calls the function attached as 'increase' to the focused line.
    menu.call_function(increase);
  }
  if (down.pressed()) {
    menu.call_function(decrease);
  }
}

void blink() {
  static bool ledOn = false;
  ledOn = !ledOn;
  ledLevel = ledOn ? 255 : 0;
  analogWrite(ledPin, ledLevel);
}

void setup() {
  Serial.begin(250000);

  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);

  led_line.attach_function(increase, increase_ledLevel);
  led_line.attach_function(decrease, decrease_ledLevel);

  // The same function attached under both numbers.
  blink_line.attach_function(increase, toggle_blinking);
  blink_line.attach_function(decrease, toggle_blinking);

  blinkPeriod_line.attach_function(increase, increase_blinkPeriod);
  blinkPeriod_line.attach_function(decrease, decrease_blinkPeriod);

  menu.update();
}

void loop() {
  checkButtons();

  static unsigned long lastMs_blink = 0;
  if (isBlinking && (millis() - lastMs_blink > blinkPeriod)) {
    lastMs_blink = millis();
    blink();
    menu.update();
  }
}
