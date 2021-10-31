![Logo][logo]

*Menu creation Arduino library for LCDs, wraps [LiquidCrystal][lc].*

[![Download](https://img.shields.io/badge/download-1.5.1-blue.svg?style=flat-square&logo=github&logoColor=white)](https://github.com/VaSe7u/LiquidMenu/archive/v1.5.1.zip)
[![Build Status](https://travis-ci.org/VaSe7u/LiquidMenu.svg?branch=master)](https://travis-ci.org/VaSe7u/LiquidMenu)
[![documentation](https://img.shields.io/badge/docs-doxygen-orange.svg)](https://VaSe7u.github.io/LiquidMenu/doc/Doxygen/html/annotated.html)
[![license](https://img.shields.io/badge/license-MIT-lightgrey.svg)](https://opensource.org/licenses/mit-license.php)

**LiquidMenu** wraps the Arduino's [LiquidCrystal][lc] library with the ability to create menus.
It simplifies the menu creation process by abstracting the elements of a menu into hierarchically organized classes.

Resources
=========
- [Examples][examples]
- [API reference][doxygen classes]
- [Latest release][latest release]

Features
========
- Fast and easy menu creation.
- Selectable menu items.
- Callback functions.
- I2C connection.

Requirements
============
- The Arduino's [LiquidCrystal][lc] or similar library.
- LCD supported by [LiquidCrystal][lc] (*with Hitachi HD44780 or a compatible chipset*).
- Arduino board or a compatible microcontroller.
- Input device is recommended (*buttons, rotary encoder, etc.*).

Quick start
===========

### Classes organization
The library uses hierarchically structured classes to represent the different elements of a menu.

*Basic class hierarchy diagram:*

![Basic class hierarchy diagram](https://github.com/VaSe7u/LiquidMenu/blob/master/doc/Images/basic_diagram.png "Basic class hierarchy diagram")

*Click [here](https://github.com/VaSe7u/LiquidMenu/blob/master/doc/Images/diagram.png?raw=true) for a complete hierarchy diagram.*

The **LiquidLine** class represents a **line** of text/numbers on the display.
To create a new **LiquidLine** object use its constructor.

The **LiquidScreen** class represents a collection of **lines** that are shown together at the same time on the display (*i.e. "the current screen"*).

The **LiquidMenu** class combines the **screens** to form a **menu**. This class is used for controlling the **menu** (_switching **screens**, selecting **lines**, calling functions etc._).

The **LiquidSystem** is an optional class that combines **menus** to form a **menu system** (*e.g. Main menu, Settings, etc.*). It has the same public interface as **LiquidMenu**.

### Creating a menu
Menu creation is all about structure. First there are variables/constants that go into the **LiquidLine** objects. Then the **LiquidLine** objects go into the **LiquidScreen** objects. Then **LiquidScreen** objects go into the **LiquidMenu** object/s. And optionally the **LiquidMenu** objects go into the **LiquidSystem** object.
This structure can be established on object instantiation or later with the classes' public methods:
```c++
// Takes column and row for the position and 1 to 4 variable references. These variable
// references are what is going to be printed on the display. They can be integers used
// in the program, string literals passed direrctly or a char* for changing text.
LiquidLine(byte column, byte row, A &variableA...);

// Takes 0 to 4 LiquidLine objects.
LiquidScreen(LiquidLine &liquidLine1...);

// Takes a reference to the LiquidCrystal object, 0 to 4 LiquidScreen objects and
// the number of the screen that will be shown first.
LiquidMenu(LiquidCrystal &liquidCrystal, LiquidScreen &liquidScreen1..., byte startingScreen = 1);

// Takes 0 to 4 LiquidMenu objects and the number of the menu that will be shown first.
LiquidSystem(LiquidMenu &liquidMenu1..., byte startingMenu = 1);
```

### Navigating the menu
The menu is navigated from the **LiquidMenu** object or if there are multiple menus - the **LiquidSystem** object. The **screens** can by cycled forward and backward or a specific **screen** can be specified by its object or number.

```c++
void LiquidMenu::next_screen();
void LiquidMenu::previous_screen();
bool LiquidMenu::change_screen(LiquidScreen &liquidScreen);
```

### Focus and callback functions
The **lines** of text/numbers shown on the display can be interactive. Every line can have callback functions attached to it (*up to 8 by default*). They are attached with a number specified by the user.
```c++
bool LiquidLine::attach_function(byte number, void (*function)(void));
```
To call a **line's** attached function, the **line** needs to be **focused** (*selected*). To cycle the **focus** through the **lines** shown on the **screen** use:
```c++
void LiquidMenu::switch_focus(bool forward = true);
```
When the **line** is selected one of its attached functions can be called with:
```c++
void LiquidMenu::call_function(byte number);
```
The `number` specifies which one of the attached functions should be called.

_Similar functions can be attached under the same number to different **lines** and then called on a similar events. For example if we are printing on the display the state of four LEDs. The four LEDs are instantiated in four **LiquidLine** objects with their name and their state. The functions used to turn them on can be attached under number 1 and the functions for turning them off - under number 2. Then if we have 3 buttons, one can be used to switch the focus, the second (say 'UP') button can be used to call function 1 and the third (say 'DOWN') button can be used to call function 2._

### Basic example
```c++
...
// First we need to instantiate the LiquidCrystal object.
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// ----- WELCOME SCREEN -----
/// Instantiating a line with one string literal.
LiquidLine welcome_line1(1, 0, "Hello Menu");

/// Instantiating a line with an integer variable.
byte oneTwoThree = 123;
LiquidLine welcome_line2(2, 1, oneTwoThree);

/// Forming a screen from the above two lines.
LiquidScreen welcome_screen(welcome_line1, welcome_line2);
// --------------------------

// ----- SCREEN 2 -----
LiquidLine some_line(0, 0, "Some line");
LiquidScreen some_screen(some_line);
// --------------------

// Now let's combine the screens into a menu.
LiquidMenu my_menu(lcd, welcome_screen, some_screen);

void setup() {
    lcd.begin(16, 2);
    ...
}

void loop() {
    if (rightButton) {
        my_menu.next_screen();
    }
    if (leftButton) {
        my_menu.previous_screen();
    }
    if (somethingElse) {
        oneTwoThree++;
        my_menu.update;
    }
    ...
}

```

Contributors
============
[Richard Wardlow](https://github.com/circuitsforfun) - Scrolling lines and configuring the number of digits shown after the decimal point.

[Jose Manuel](https://github.com/jmpmscorp) - Getter functions in "lines".

For more information, please refer to the [CONTRIBUTING][contributing] guide.

License
=======
The MIT License (MIT)

Copyright (c) 2016 Vasil Kalchev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

[logo]: /doc/Images/logo.png
[lc]: https://github.com/arduino-libraries/LiquidCrystal
[doxygen classes]: https://VaSe7u.github.io/LiquidMenu/doc/Doxygen/html/annotated.html
[examples]: /examples
[latest release]: https://github.com/VaSe7u/LiquidMenu/releases/latest
[code_of_conduct]: /.github/CODE_OF_CONDUCT.md
[contributing]: /.github/CONTRIBUTING.md
