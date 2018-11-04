/*
 * LiquidMenu library - progmem_menu.ino
 *
 * This example demonstrates how to display a string stored in flash memory.
 *
 * Usually all the variables are stored in RAM (which is 2KB on ATmega328).
 * To conserve RAM, big variables like char arrays can be stored along with
 * the program in flash memory (which is 32KB on ATmega328). There is a tutorial
 * on how to do that on the Arduino web site: https://www.arduino.cc/en/Reference/PROGMEM,
 * there is also a very good tutorial here: http://www.gammon.com.au/progmem.
 * Basically the variable is prepended with the "const" variable modifier and
 * appended with the "PROGMEM" keyword. After that the variable is added to the
 * LiquidLine object as usual. The difference is that later in the setup(), the method
 * bool LiquidLine::set_asProgmem(byte number) must be called on the LiquidLine object
 * holding the variable. The "number" parameter is the consecutive number of the variable
 * for this LiquidLine object. If there are two variables, the first is an integer
 * and the second is a string stored in flash, "number" will be 2.
 *
 * The circuit:
 * https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/progmem_menu/E_progmem_menu.png
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
#include <avr/pgmspace.h> // The PROGMEM library

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


// These are the char arrays stored in flash
const char text1[] PROGMEM = "Lorem ipsum dolo";
const char text2[] PROGMEM = "r sit amet, cons";
const char text3[] PROGMEM = "ectetur adipisci";
const char text4[] PROGMEM = "ibendum, sem ac ";
const char text5[] PROGMEM = "venenatis rhoncu";
const char text6[] PROGMEM = "s, nisi justo eu";
const char text7[] PROGMEM = "ismod lectus, eu";
const char text8[] PROGMEM = " eleifend ante n";
const char text9[] PROGMEM = "isl vitae eros. ";
const char text10[] PROGMEM = "Pellentesque hab";
const char text11[] PROGMEM = "itant morbi tris";
const char text12[] PROGMEM = "tique senectus e";
const char text13[] PROGMEM = "t netus et males";
const char text14[] PROGMEM = "uada fames ac tu";
const char text15[] PROGMEM = "rpis egestas. Or";
const char text16[] PROGMEM = "ci varius natoqu";
const char text17[] PROGMEM = "e penatibus et m";
const char text18[] PROGMEM = "agnis dis partur";
const char text19[] PROGMEM = "ient montes, nas";
const char text20[] PROGMEM = "cetur ridiculus ";

LiquidLine welcome_line1(1, 0, "LiquidMenu ", LIQUIDMENU_VERSION);
LiquidLine welcome_line2(0, 1, "PROGMEM example");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine line11(0, 0, text1);
LiquidLine line12(0, 1, text2);
LiquidScreen screen1(line11, line12);

LiquidLine line21(0, 0, text3);
LiquidLine line22(0, 1, text4);
LiquidScreen screen2(line21, line22);

LiquidLine line31(0, 0, text5);
LiquidLine line32(0, 1, text6);
LiquidScreen screen3(line31, line32);

LiquidLine line41(0, 0, text7);
LiquidLine line42(0, 1, text8);
LiquidScreen screen4(line41, line42);

LiquidLine line51(0, 0, text9);
LiquidLine line52(0, 1, text10);
LiquidScreen screen5(line51, line52);

LiquidLine line61(0, 0, text11);
LiquidLine line62(0, 1, text12);
LiquidScreen screen6(line61, line62);

LiquidLine line71(0, 0, text13);
LiquidLine line72(0, 1, text14);
LiquidScreen screen7(line71, line72);

LiquidLine line81(0, 0, text15);
LiquidLine line82(0, 1, text16);
LiquidScreen screen8(line81, line82);

LiquidLine line91(0, 0, text17);
LiquidLine line92(0, 1, text18);
LiquidScreen screen9(line91, line92);

LiquidLine line101(0, 0, text19);
LiquidLine line102(0, 1, text20);
LiquidScreen screen10(line101, line102);

LiquidMenu menu(lcd);


void setup() {
  Serial.begin(250000);

  lcd.begin(16, 2);

  // Set the variables as PROGMEM. The parameter is the consecutive
  // number of the PROGMEM variable for that LiquidLine object.
  line11.set_asProgmem(1);
  line12.set_asProgmem(1);
  line21.set_asProgmem(1);
  line22.set_asProgmem(1);
  line31.set_asProgmem(1);
  line32.set_asProgmem(1);
  line41.set_asProgmem(1);
  line42.set_asProgmem(1);
  line51.set_asProgmem(1);
  line52.set_asProgmem(1);
  line61.set_asProgmem(1);
  line62.set_asProgmem(1);
  line71.set_asProgmem(1);
  line72.set_asProgmem(1);
  line81.set_asProgmem(1);
  line82.set_asProgmem(1);
  line91.set_asProgmem(1);
  line92.set_asProgmem(1);
  line101.set_asProgmem(1);
  line102.set_asProgmem(1);

  menu.add_screen(welcome_screen);
  menu.add_screen(screen1);
  menu.add_screen(screen2);
  menu.add_screen(screen3);
  menu.add_screen(screen4);
  menu.add_screen(screen5);
  menu.add_screen(screen6);
  menu.add_screen(screen7);
  menu.add_screen(screen8);
  menu.add_screen(screen9);
  menu.add_screen(screen10);
}

void loop() {
  static unsigned long lastMs_nextScreen = 0;
  static unsigned int period_nextScreen = 1000;
  // Periodic switching to the next screen.
  if (millis() - lastMs_nextScreen > period_nextScreen) {
    lastMs_nextScreen = millis();
    menu.next_screen();
  }
}
