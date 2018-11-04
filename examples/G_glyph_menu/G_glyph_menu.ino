/*
 * LiquidMenu library - glyph_menu.ino
 *
 * This example demonstrates how to create a custom character
 * (glyph) and put it inside a LiquidLine object.
 *
 * The LiquidCrystal library allows creating custom characters
 * with the method bool LiquidCrystal::createChar(byte num, byte data[]).
 * The "num" parameter is an index (0 to 7), identifying the glyph and
 * the "data" parameter is an array holding the actual glyph. To insert
 * it in a LiquidLine object put the "num" parameter (used when creating the
 * glyph earlier) in the LiquidLine object as a normal variable or an int
 * literal (e.g. LiquidLine line1(0, 0, glyphNum)) and then call the method
 * bool LiquidLine::set_asGlyph(byte number) where "number" is the consecutive
 * number of the variable for that line (e.g. line1.set_asGlyph(1)). Now when
 * displaying the line on the screen the object will know that this number
 * should not be displayed directly but is referencing a created character.
 * The glyph can be animated by changing the value of the variable pointing
 * to the created character to point to other created character. In the case
 * of animating a glyph, the method void LiquidMenu::softUpdate() can be used
 * to update the display to the new glyph without clearing the display. This
 * method can be used in any case when you are sure that the new information
 * shown on the display will overwrite the old.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/glyph_menu/G_glyph_menu.png
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
 * http://omerk.github.io/lcdchargen/ - for creating custom character definition
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
//10K potentiometer to VO
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// The glyph are separated in a namespace. To use them use
// namespace_name::variable_name.
namespace glyphs {
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

uint8_t celsiusSymbol[8] = {
  0b00011,
  0b00011,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

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
} //namespace glyphs

// These are the indexes we will use to create the characters
// and set them as variables of a LiquidLine object.
byte fan_glyphIndex = 0;
byte thermometer_glyphIndex = 2;
byte celsiusSymbol_glyphIndex = 3;

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(0, 1, "PROGMEM example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine line1(1, 1, fan_glyphIndex);
LiquidLine line2(7, 1, thermometer_glyphIndex);
LiquidLine line3(13, 1, celsiusSymbol_glyphIndex, "C");
LiquidScreen screen1(line1, line2, line3);

LiquidMenu menu(lcd, welcome_screen, screen1);

void setup() {
  Serial.begin(250000);

  // Here we create the characters using the LiquidCrystal library.
  // The fan glyph will be animated. This is the first glyph.
  lcd.createChar(fan_glyphIndex, glyphs::fan1);
  // This is the second glyph, indexed after the first.
  lcd.createChar(fan_glyphIndex + 1, glyphs::fan2);
  lcd.createChar(thermometer_glyphIndex, glyphs::thermometer);
  lcd.createChar(celsiusSymbol_glyphIndex, glyphs::celsiusSymbol);

  // This functions tells the LiquidLine objects that their first
  // variable is not an integer value but an index to a glyph.
  line1.set_asGlyph(1);
  line2.set_asGlyph(1);
  line3.set_asGlyph(1);

  lcd.begin(16, 2);
}

void loop() {
  // This block cycles the value of the variable pointing to
  // one of the fan glyphs to point to the other.
  static unsigned long lastMs_fanAnimation = 0;
  static unsigned int period_fanAnimation = 500;
  if (millis() - lastMs_fanAnimation > period_fanAnimation) {
    lastMs_fanAnimation = millis();
    static bool currentAnimation_fan = 0;
    if (currentAnimation_fan == 0) {
      currentAnimation_fan = 1;
      fan_glyphIndex = 1;
    } else {
      currentAnimation_fan = 0;
      fan_glyphIndex = 0;
    }
    // The display must be updated to show the new glyph.
    menu.softUpdate();
  }

  static unsigned long lastMs_nextScreen = 0;
  static unsigned int period_nextScreen = 4000;
  if (millis() - lastMs_nextScreen > period_nextScreen) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
