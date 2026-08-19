/*
 * LiquidMenu library - C_buttons_menu.ino (ESP32)
 *
 * The interactive menu on an ESP32: five buttons navigate the screens,
 * move the focus and call functions that dim an LED.
 *
 * The menu part is identical to the Arduino version of this example -
 * lines get callback functions attached under a number, the focused
 * line's function is the one that runs. What is worth knowing on an
 * ESP32:
 *
 *  - INPUT_PULLUP works on the regular GPIOs, but NOT on GPIO 34-39:
 *    those are input only and have no internal pull-ups, so a button on
 *    one of them needs an external resistor. The buttons here are on
 *    regular pins,
 *  - analogWrite() exists in the ESP32 Arduino core 3.x. On the 2.x core
 *    use the LEDC API instead:
 *      ledcSetup(0, 5000, 8); ledcAttachPin(ledPin, 0);
 *      ledcWrite(0, level);
 *  - the display is on I2C here, which is the usual way on an ESP32.
 *    Set the library to the I2C display library in
 *    "LiquidMenu_config.h" - see the "B_I2C_menu" example.
 *
 * Try it: press RIGHT until the "LED" screen, press ENTER to focus the
 * line, then UP/DOWN to dim the LED.
 *
 * Simulate it online:
 * Open this folder's "diagram.json" in Wokwi - see ../../README.md.
 *
 * The circuit:
 * - I2C module SDA to ESP32 GPIO 21
 * - I2C module SCL to ESP32 GPIO 22
 * - I2C module VCC to ESP32 5V (VIN)
 * - I2C module GND to ESP32 GND
 * - ----
 * - Button (left) to ESP32 GPIO 32 and ground
 * - Button (right) to ESP32 GPIO 33 and ground
 * - Button (up) to ESP32 GPIO 25 and ground
 * - Button (down) to ESP32 GPIO 26 and ground
 * - Button (enter) to ESP32 GPIO 27 and ground
 * - An LED (with a resistor) to ESP32 GPIO 4 and ground
 *
 * Created August 20, 2026
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/LiquidMenu
 *
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>
#include "Button.h"

// The I2C address, the columns and the rows of the display.
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte I2C_SDA = 21;
const byte I2C_SCL = 22;

// The Button class is not a part of LiquidMenu (see "Button.h"). These
// GPIOs have internal pull-ups, unlike GPIO 34-39.
Button left(32);
Button right(33);
Button up(25);
Button down(26);
Button enter(27);

enum FunctionTypes {
  increase = 1,
  decrease = 2,
};

const byte ledPin = 4;
byte ledLevel = 0;

bool isBlinking = false;
char blinking_text[4] = "OFF";
// A fixed width type: on a 32 bit core a plain "unsigned int" is
// ambiguous for the library's type recognition.
uint16_t blinkPeriod = 500;


LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "ESP32 buttons");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine led_title_line(6, 0, "LED");
LiquidLine led_line(4, 1, "Level: ", ledLevel);
LiquidScreen led_screen(led_title_line, led_line);

LiquidLine blink_line(0, 0, "Blink - ", blinking_text);
LiquidLine blinkPeriod_line(0, 1, "Period: ", blinkPeriod, "ms");
LiquidScreen blink_screen(blink_line, blinkPeriod_line);

LiquidMenu menu(lcd, welcome_screen, led_screen, blink_screen);


// Wraps the PWM output so that the core version difference stays in one
// place. See the note at the top for the 2.x core.
void write_led(byte level) {
  analogWrite(ledPin, level);
}

void increase_ledLevel() {
  if (ledLevel <= 230) {
    ledLevel += 25;
  }
  write_led(ledLevel);
}

void decrease_ledLevel() {
  if (ledLevel >= 25) {
    ledLevel -= 25;
  }
  write_led(ledLevel);
}

// Attached under both numbers - UP and DOWN both toggle the blinking.
void toggle_blinking() {
  isBlinking = !isBlinking;
  strcpy(blinking_text, isBlinking ? "ON" : "OFF");
  ledLevel = 0;
  write_led(ledLevel);
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
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  Wire.begin(I2C_SDA, I2C_SCL);

  lcd.init();
  lcd.backlight();

  // Required with an I2C display, after the display's own init.
  menu.init();

  led_line.attach_function(increase, increase_ledLevel);
  led_line.attach_function(decrease, decrease_ledLevel);

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
    static bool ledOn = false;
    ledOn = !ledOn;
    ledLevel = ledOn ? 255 : 0;
    write_led(ledLevel);
    menu.update();
  }
}
