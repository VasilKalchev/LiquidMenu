![Logo][logo-img]

<p align=center>
  <i>Menu creation Arduino library for LCDs, wraps <a href="https://github.com/arduino-libraries/LiquidCrystal">LiquidCrystal</a>.</i>
</p>

<p align="center">
  <a href="https://github.com/VaSe7u/LiquidMenu/archive/v1.6.0.zip"><img alt="Download" src="https://img.shields.io/badge/download-1.6.0-blue.svg?style=flat-square&logo=github&logoColor=white&color=1cb3e4" /></a>
  <a href="https://VaSe7u.github.io/LiquidMenu/doc/Doxygen/html/annotated.html"><img alt="Documentation" src="https://img.shields.io/badge/docs-doxygen-blue.svg?style=flat-square" /></a>
  <a href="https://github.com/VaSe7u/LiquidMenu/actions/workflows/compile-examples.yml"><img alt="GitHub workflow - compile examples" src="https://img.shields.io/github/workflow/status/vase7u/LiquidMenu/compile%20examples?label=build%20examples&style=flat-square" /></a>
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github.com/vase7u/liquidmenu?style=flat-square&color=orange" /></a>
  <a href="https://choosealicense.com/licenses/mit/"><img alt="MIT license" src="https://img.shields.io/badge/license-MIT-lightgrey.svg?style=flat-square" /></a>
</p>

**LiquidMenu** wraps Arduino's [LiquidCrystal][LiquidCrystal-lib] library with the ability to create menus.
It simplifies the menu creation process by abstracting the elements of a menu into hierarchically organized classes.


## Features
- fast and easy menu creation
- selectable menu items
- callback functions
- parallel or I2C connection

## Resources
 - [Examples][examples-dir]
 - [API reference][doxygen-doc]

## Installation
Use _Arduino's library manager_ (recommended) or download it directly from [here][latest_release].

<!-- </br> -->

# Quick start
### Requirements
- Arduino's [LiquidCrystal][LiquidCrystal-lib] or similar library
- LCD supported by [LiquidCrystal][LiquidCrystal-lib] (with Hitachi HD44780 or a compatible chipset)
- Arduino board or a compatible microcontroller
- push-buttons or something similar

### Classes organization
The library uses hierarchically structured classes to represent the different elements of a menu.

#### Basic classes structure diagram:
![basic classes structure diagram](https://raw.githubusercontent.com/VaSe7u/LiquidMenu/master/doc/img/classes-structure/menu.png "Basic classes structure diagram")

The `LiquidLine` class represents a *line* of text/numbers on the display.

The `LiquidScreen` class represents a collection of *lines* that are shown together on the display (i.e. the current screen).

The `LiquidMenu` class combines the *screens* to form a *menu*. This class is used for controlling the *menu* (switching between *screens*, selecting *lines*, calling functions etc...).

If your structure consists of multiple different *menus*, for example a "Main menu" that displays sensor information on multiple screens and a "Settings" menu for configuring the sensors, you can combine the different *menus* inside a `LiquidSystem` class. That way, you can browse the screens of just the *menu* you are currently on. `LiquidSystem` has the same public interface as `LiquidMenu`. Sub-menus [classes structure diagram](https://raw.githubusercontent.com/VaSe7u/LiquidMenu/master/doc/img/classes-structure/system.png).


### Creating a menu
Menu creation is all about structure. First there are variables/constants that go into the `LiquidLine` objects. Then the `LiquidLine` objects go into the `LiquidScreen` objects. Then `LiquidScreen` objects go into the `LiquidMenu` object(s). And optionally the `LiquidMenu` objects go into the `LiquidSystem` object.
This structure can be established either on object instantiation or later using the classes' methods.
```c++
// Takes column and row for the position and 1 to 4 variable references. These variable
// references are what is going to be printed on the display. They can be integers,
// string literals or char[] arrays for dynamic text.
LiquidLine(byte column, byte row, A &variableA...);

// Takes 0 to 4 LiquidLine objects.
LiquidScreen(LiquidLine &liquidLine1...);

// Takes a reference to the LiquidCrystal object, 0 to 4 LiquidScreen objects and
// optionally the number of the screen that will be shown first.
LiquidMenu(LiquidCrystal &liquidCrystal, LiquidScreen &liquidScreen1..., byte startingScreen = 1);

// Takes 0 to 4 LiquidMenu objects and optionally the number of the menu that will be shown first.
LiquidSystem(LiquidMenu &liquidMenu1..., byte startingMenu = 1);
```

### Navigating the menu
The menu is navigated from the `LiquidMenu` object or if there are multiple menus - the `LiquidSystem` object. The *screens* can by cycled forward and backward, it's also possible to jump to a specific *screen* using its object or consecutive number:
```c++
void LiquidMenu::next_screen();
void LiquidMenu::previous_screen();
bool LiquidMenu::change_screen(LiquidScreen &liquidScreen);
```

### Focus and callback functions
The *lines* of text/numbers shown on the display can be made interactive. Every line can have callback functions attached to it (up to 8 by default). Every attached callback function must be identified by number:
```c++
bool LiquidLine::attach_function(byte number, void (*function)(void));
```

To call a *line's* attached function, the *line* needs to be *focused* (selected). To cycle the *focus* through the *lines* shown on the *screen* use:
```c++
void LiquidMenu::switch_focus(bool forward = true);
```

When the *line* is selected one of its attached functions can be called with:
```c++
void LiquidMenu::call_function(byte number);
```
The `number` specifies which one of the attached functions should be called.

> Conceptionally similar functions should be attached under the same number to the different *lines*. For example if we are printing the state of four LEDs. Each LEDis instantiated in a `LiquidLine` object with their name and their state. The functions used to turn them on can be attached under number *1* and the functions for turning them off - under number *2*. Then if we have 3 buttons, one can be used to switch the focus, the second (say 'UP') button can be used to call function *1* and the third (say 'DOWN') button can be used to call function *2*.

### Basic example
```c++
// ...

// First we need to instantiate the LiquidCrystal object.
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// a variable that will be updated periodically somewhere in our code
float temperature = 0.0f;

// ----- WELCOME SCREEN -----
// a line of one string literal
LiquidLine lineHello(2, 0, "Hello world!");
LiquidLine lineRight(7, 1, ">");

// create a screen from the above lines
LiquidScreen screenMain(lineHello, lineRight);
// --------------------------

// ----- STATUS SCREEN -----
// a line of two string literals and a float variable
LiquidLine lineTemp(0, 0, "Temp: ", temperature, "C");
LiquidScreen screenStatus(lineTemp);
// -------------------------

// ----- MENU -----
// create a menu from the screens
LiquidMenu menu(lcd, screenMain, screenStatus);
// ----------------

void setup() {
    lcd.begin(16, 2);
    
    // ...
}

void loop() {
    temperature = readTemperature();
    menu.update(); // update the display to show the new information
    
    if (rightButton()) {
        menu.next_screen();
    }
    if (leftButton()) {
        menu.previous_screen();
    }
    
    // ...
}
```

# Community and contributing
:grey_question: If you have a question head over to [Discussions - Q&A][q&a-disc].

:bug: If you encounter a bug, feel free to open an [issue][issues].

:bulb: To propose a feature go to [Discussions - Ideas][ideas-disc].

:sunglasses: You are also welcome to show off your project in [Discussions - Show & Tell][show&tell-disc].

All other forms of contributions (bug fixes, feature implementations, documentation improvements, typos, etc...) are welcome. To get started see the [contributing][contributing-doc] guide.


## Contributors
 - [Richard Wardlow](https://github.com/circuitsforfun) - Scrolling lines and configuring the number of digits shown after the decimal point.
 - [jmpmscorp](https://github.com/jmpmscorp) - Getter functions in *lines*.
 - [RobotGrrl](https://github.com/RobotGrrl) - Fixing bugs in `LiquidSystem`.
 - [thijstriemstra](https://github.com/thijstriemstra) and [DarioMaechler](https://github.com/DarioMaechler) - Configurable focus indicator "ghosting".
 - [per1234](https://github.com/per1234) - Fixing configuration files problems.


# License
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


[logo-img]: /doc/img/logo/logo.png
[LiquidCrystal-lib]: https://github.com/arduino-libraries/LiquidCrystal
[doxygen-doc]: https://VaSe7u.github.io/LiquidMenu/doc/Doxygen/html/annotated.html
[examples-dir]: /examples
[q&a-disc]: https://github.com/VaSe7u/LiquidMenu/discussions/categories/q-a
[ideas-disc]: https://github.com/VaSe7u/LiquidMenu/discussions/categories/ideas
[show&tell-disc]: https://github.com/VaSe7u/LiquidMenu/discussions/categories/show-tell
[issues]: https://github.com/VaSe7u/LiquidMenu/issues/new/choose
[latest_release]: https://github.com/VaSe7u/LiquidMenu/releases/latest
[code_of_conduct-doc]: /.github/CODE_OF_CONDUCT.md
[contributing-doc]: /.github/CONTRIBUTING.md
