/*
 * LiquidMenu library - F_glyph_menu.ino
 *
 * Printing custom characters (glyphs) and animating them.
 *
 * An HD44780 display can hold eight user defined 5x8 characters. They
 * are created with the wrapped library:
 *
 *   lcd.createChar(index, glyphData);   // index is 0 to 7
 *
 * and printed by writing the index. To put one in a menu, pass the
 * index variable to the LiquidLine object like any other variable and
 * then tell the line that this variable is a glyph index rather than a
 * number to print:
 *
 *   line.set_asGlyph(number);   // number - which variable of the line
 *
 * Because the line holds a reference to the index variable, assigning a
 * different index to it makes the line show a different glyph - that is
 * all an animation is. Refreshing an animation is a good use for
 * menu.softUpdate(), which redraws without clearing the display first
 * and so avoids the flicker of menu.update(). It is safe whenever the
 * new content is known to cover the old one.
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
 * http://omerk.github.io/lcdchargen/ - for creating the glyphs
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

// The glyph definitions are kept in a namespace to keep the sketch's
// global scope tidy. Use them as glyphs::name.
namespace glyphs {
uint8_t fan1[8] = {
  0b00000,
  0b11001,
  0b01011,
  0b00100,
  0b11010,
  0b10011,
  0b00000,
  0b00000
};

uint8_t fan2[8] = {
  0b00000,
  0b10011,
  0b11010,
  0b00100,
  0b01011,
  0b11001,
  0b00000,
  0b00000
};

uint8_t thermometer[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b01010,
  0b10001,
  0b11111,
  0b01110,
  0b00000
};

uint8_t degreeSymbol[8] = {
  0b00011,
  0b00011,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
} // namespace glyphs

/*
 * The indexes the glyphs are created under. The fan is animated, so it
 * takes two of them: 'fan_glyph' holds whichever of the two is shown at
 * the moment, which is why it is not const.
 */
byte fan_glyph = 0;                  // the second frame is index 1
const byte thermometer_glyph = 2;
const byte degree_glyph = 3;

int temperature = 21;

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(1, 1, "Glyphs example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine fan_line(1, 1, fan_glyph);
LiquidLine thermometer_line(7, 1, thermometer_glyph);
LiquidLine temperature_line(9, 1, temperature, degree_glyph, "C");
LiquidScreen status_screen(fan_line, thermometer_line, temperature_line);

LiquidMenu menu(lcd, welcome_screen, status_screen);


void setup() {
  Serial.begin(250000);

  lcd.begin(16, 2);

  // Creating the characters with the wrapped library.
  lcd.createChar(fan_glyph, glyphs::fan1);
  lcd.createChar(fan_glyph + 1, glyphs::fan2);
  lcd.createChar(thermometer_glyph, glyphs::thermometer);
  lcd.createChar(degree_glyph, glyphs::degreeSymbol);

  /*
   * Tells the lines that the given variable holds a glyph index and
   * should not be printed as a number. The parameter is the consecutive
   * number of the variable for that line - the degree symbol is the
   * second variable of 'temperature_line'.
   */
  fan_line.set_asGlyph(1);
  thermometer_line.set_asGlyph(1);
  temperature_line.set_asGlyph(2);

  menu.update();
}

void loop() {
  // Animating the fan: swap the index the line points to.
  static unsigned long lastMs_animation = 0;
  if (millis() - lastMs_animation > 500) {
    lastMs_animation = millis();
    fan_glyph = (fan_glyph == 0) ? 1 : 0;
    // Redraw without clearing the display first - no flicker.
    menu.softUpdate();
  }

  static unsigned long lastMs_nextScreen = 0;
  if (millis() - lastMs_nextScreen > 4000) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
