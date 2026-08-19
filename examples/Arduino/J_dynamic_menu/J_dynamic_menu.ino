/*
 * LiquidMenu library - J_dynamic_menu.ino
 *
 * Showing and hiding screens while the sketch runs.
 *
 * A menu does not have to be fixed. A screen can be taken out of the
 * rotation and put back in with:
 *
 *   screen.hide(true);   // next_screen()/previous_screen() skip it
 *   screen.hide(false);  // it is back in the cycle
 *
 * That is how an "advanced settings" section that only appears when it
 * is enabled, or a screen for a sensor that is not connected, is built -
 * the screens are all created up front and only their visibility
 * changes.
 *
 * This example also shows the two remaining ways of moving around a
 * menu: change_screen(), which jumps straight to a screen, and
 * get_currentScreen(), which tells where the menu is at the moment.
 *
 * Note that at least one screen must stay visible - hiding every screen
 * leaves next_screen() with nothing to switch to.
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
 *
 * Created August 20, 2026
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

// The Button class is not a part of LiquidMenu (see "Button.h").
Button left(A0);
Button right(7);
Button up(8);
Button down(9);
Button enter(10);

enum FunctionTypes {
  increase = 1,
  decrease = 2,
};

bool advancedShown = false;
char advanced_text[4] = "OFF";

unsigned int calibration = 100;


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "Dynamic menu");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine advanced_line(0, 0, "Advanced: ", advanced_text);
LiquidLine hint_line(0, 1, "UP/DOWN toggles");
LiquidScreen main_screen(advanced_line, hint_line);

// These two are hidden until "Advanced" is turned on.
LiquidLine calibration_line(0, 0, "Calibr.: ", calibration);
LiquidScreen calibration_screen(calibration_line);

LiquidLine home_line(0, 0, "Back to start");
LiquidScreen home_screen(home_line);

LiquidMenu menu(lcd, welcome_screen, main_screen,
                calibration_screen, home_screen);


// Shows or hides the advanced screens.
void toggle_advanced() {
  advancedShown = !advancedShown;
  strcpy(advanced_text, advancedShown ? "ON" : "OFF");

  calibration_screen.hide(!advancedShown);
  home_screen.hide(!advancedShown);
}

void increase_calibration() {
  if (calibration <= 990) {
    calibration += 10;
  }
}

void decrease_calibration() {
  if (calibration >= 10) {
    calibration -= 10;
  }
}

// Jumps straight to a screen instead of cycling to it.
void go_home() {
  menu.change_screen(&welcome_screen);
}

void checkButtons() {
  if (right.pressed()) {
    menu.next_screen();
  }
  if (left.pressed()) {
    menu.previous_screen();
  }
  if (enter.pressed()) {
    menu.switch_focus();
  }
  if (up.pressed()) {
    menu.call_function(increase);
  }
  if (down.pressed()) {
    menu.call_function(decrease);
  }
}

void setup() {
  Serial.begin(250000);

  lcd.begin(16, 2);

  // Both buttons toggle - this line is a switch, not a value.
  advanced_line.attach_function(increase, toggle_advanced);
  advanced_line.attach_function(decrease, toggle_advanced);

  calibration_line.attach_function(increase, increase_calibration);
  calibration_line.attach_function(decrease, decrease_calibration);

  home_line.attach_function(increase, go_home);
  home_line.attach_function(decrease, go_home);

  // The advanced screens start out hidden.
  calibration_screen.hide(true);
  home_screen.hide(true);

  menu.update();
}

void loop() {
  checkButtons();

  // Report where the menu is whenever it moves.
  static LiquidScreen *lastScreen = nullptr;
  LiquidScreen *currentScreen = menu.get_currentScreen();
  if (currentScreen != lastScreen) {
    lastScreen = currentScreen;
    if (currentScreen == &calibration_screen) {
      Serial.println(F("On the calibration screen"));
    }
  }
}
