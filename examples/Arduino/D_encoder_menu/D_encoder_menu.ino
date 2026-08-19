/*
 * LiquidMenu library - D_encoder_menu.ino
 *
 * Navigating and editing with a single rotary encoder.
 *
 * A rotary encoder with a push-button is the smallest input that can
 * drive a full menu, and it needs one extra idea: an "edit mode". The
 * same rotation has to do two different things, so the sketch keeps a
 * flag:
 *
 *   - not editing - rotating moves the focus from line to line, pressing
 *     the knob either enters edit mode or, on a line whose callback does
 *     something else, performs that action (here: change the screen),
 *   - editing - rotating calls the focused line's 'increase' or
 *     'decrease' callback, pressing the knob leaves edit mode.
 *
 * Which mode you are in is shown by the focus indicator: the sketch
 * swaps the focus symbol with menu.set_focusSymbol() when the mode
 * changes, so the arrow turns into a pencil-like glyph while editing.
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
 * - Rotary encoder CLK pin to Arduino pin 7
 * - Rotary encoder DT pin to Arduino pin 8
 * - Rotary encoder SW pin to Arduino pin 9
 * - Rotary encoder GND pin to ground
 * - An LED (with a resistor) to Arduino pin 6 and ground
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
#include "Encoder.h"

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

// The Encoder and Button classes are not a part of LiquidMenu, they are
// bundled with the example (see "Encoder.h" and "Button.h").
Encoder knob(7, 8);
Button knobSwitch(9);

enum FunctionTypes {
  increase = 1,
  decrease = 2,
  select = 3,
};

// True while the focused value is being edited.
bool editing = false;

const byte ledPin = 6;
byte brightness = 0;
unsigned int blinkPeriod = 500;

// The focus indicator symbols: an arrow while navigating and a filled
// block while editing. http://omerk.github.io/lcdchargen/
uint8_t navigateSymbol[8] = {
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b00110,
  0b00100,
  0b00000,
  0b00000
};

uint8_t editSymbol[8] = {
  0b00000,
  0b00010,
  0b00111,
  0b01110,
  0b11100,
  0b11000,
  0b00000,
  0b00000
};


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "Turn the knob");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine brightness_line(0, 0, "Bright: ", brightness);
LiquidLine period_line(0, 1, "Blink:  ", blinkPeriod, "ms");
LiquidScreen settings_screen(brightness_line, period_line);

// A line whose callback is an action rather than a value change.
LiquidLine back_line(0, 0, "Back to start");
LiquidScreen back_screen(back_line);

LiquidMenu menu(lcd, welcome_screen, settings_screen, back_screen);


void increase_brightness() {
  if (brightness <= 230) {
    brightness += 25;
  }
  analogWrite(ledPin, brightness);
}

void decrease_brightness() {
  if (brightness >= 25) {
    brightness -= 25;
  }
  analogWrite(ledPin, brightness);
}

void increase_period() {
  if (blinkPeriod <= 2900) {
    blinkPeriod += 100;
  }
}

void decrease_period() {
  if (blinkPeriod >= 200) {
    blinkPeriod -= 100;
  }
}

// Attached under 'select', so pressing the knob on this line runs it
// instead of entering edit mode.
void go_to_start() {
  menu.change_screen(&welcome_screen);
}

// Swaps the focus symbol to show which mode the knob is in.
void set_editing(bool state) {
  editing = state;
  menu.set_focusSymbol(Position::RIGHT, editing ? editSymbol : navigateSymbol);
  menu.update();
}

void setup() {
  Serial.begin(250000);

  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);

  brightness_line.attach_function(increase, increase_brightness);
  brightness_line.attach_function(decrease, decrease_brightness);
  period_line.attach_function(increase, increase_period);
  period_line.attach_function(decrease, decrease_period);
  back_line.attach_function(select, go_to_start);

  // A line needs an attached function to be focusable - this one gets
  // the focus so that the knob can be pressed on it.
  welcome_line2.attach_function(select, go_to_start);

  menu.set_focusSymbol(Position::RIGHT, navigateSymbol);

  menu.update();
}

void loop() {
  int8_t rotation = knob.read();

  if (rotation != 0) {
    if (editing) {
      // In edit mode the rotation changes the focused line's value.
      menu.call_function(rotation > 0 ? increase : decrease);
    } else {
      /*
       * Otherwise it moves the focus. With the focus indicator
       * "ghosting" (enabled by default in "LiquidMenu_config.h") there
       * is one step in the cycle where no line is focused - that step is
       * used here to move on to the next screen.
       */
      menu.switch_focus(rotation > 0);
      bool aLineIsFocused = menu.is_callable(increase) || menu.is_callable(select);
      if (!aLineIsFocused) {
        (rotation > 0) ? menu.next_screen() : menu.previous_screen();
      }
    }
  }

  if (knobSwitch.pressed()) {
    if (editing) {
      set_editing(false);
    } else if (menu.is_callable(select)) {
      // The focused line has an action attached - run it.
      menu.call_function(select);
    } else if (menu.is_callable(increase)) {
      // The focused line holds an editable value.
      set_editing(true);
    }
  }
}
