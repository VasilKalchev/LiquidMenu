![logo](https://raw.githubusercontent.com/VasilKalchev/LiquidMenu/master/doc/logo.png)

*Menu creation Arduino library for LCDs, extends [LiquidCrystal][lc].*

[![release](https://img.shields.io/badge/release-1.0.0-yellow.svg)](https://github.com/VasilKalchev/LiquidMenu/releases)
[![documentation](https://img.shields.io/badge/docs-doxygen-green.svg)](https://vasilkalchev.github.io/LiquidMenu/doc/html/index.html)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://opensource.org/licenses/mit-license.php)


**LiquidMenu** extends the Arduino's [LiquidCrystal][lc] library with the ability to create menus.
It simplifies the menu creation process by abstracting the elements of a menu into hierarchically organized classes.


Features
========
 - Fast and easy menu creation.
 - Selectable menu items.
 - Callback functions.


Requirements
============
 - The Arduino's [LiquidCrystal][lc] library.
 - LCD supported by <a href="https://github.com/arduino/Arduino/tree/master/libraries/LiquidCrystal"> LiquidCrystal</a> (*with Hitachi HD44780 or a compatible chipset*).
 - Arduino board or a compatible microcontroller.
 - Input device/s is recommended (*buttons, rotary encoder, etc.*).

How to use it
=============
Classes organization
--------------------
This library uses hierarchically structured classes to represent the different elements of a menu.

*Basic class hierarchy diagram:*

![Basic class hierarchy diagram](https://raw.githubusercontent.com/VasilKalchev/LiquidMenu/master/doc/basic_diagram.png "Basic class hierarchy diagram")

*Click [here](https://raw.githubusercontent.com/VasilKalchev/LiquidMenu/master/doc/diagram.png) for a complete hierarchy diagram.*

The **LiquidLine** class represents a **line** of text/numbers on the display.

The **LiquidScreen** class represents a collection of **lines** that are shown together at the same time on the display (*i.e. "the current screen"*).

The **LiquidMenu** class combines the **screens** to form a **menu**. It can switch between them, call attached functions, etc.
 
The **LiquidSystem** is an optional class that combines **menus** to form a **menu system** (*e.g. Main menu, Settings, etc.*). It has the same public interface as **LiquidMenu**.


Creating a menu
---------------
Menu creation is all about structure. First there are variables/constants that go into the **LiquidLine** objects. Then the **LiquidLine** objects go into the **LiquidScreen** objects. Then **LiquidScreen** objects go into the **LiquidMenu** object/s. And optionally the **LiquidMenu** objects go into the **LiquidSystem** object.
This structure can be established on object instantiation or later with functions:

#### Methods used:
```c++
LiquidLine(byte column, byte row, A &variableA...);
```
**LiquidLine's** contructor. Takes a column, a row and 1 to 4 variable references.


```c++
LiquidScreen(LiquidLine &liquidLine1...);
```
**LiquidScreen's** constructor. Takes 0 to 4 **LiquidLine** objects.


```c++
LiquidMenu(LiquidCrystal &liquidCrystal, LiquidScreen &liquidScreen1..., byte startingScreen = 1);
```
**LiquidMenu's** constructor. Takes a reference to the **LiquidCrystal** object, 0 to 4 **LiquidScreen** objects and the number of the screen that will be shown first.


```c++
LiquidSystem(LiquidMenu &liquidMenu1..., byte startingMenu = 1);
```
**LiquidSystem's** constructor. Takes 0 to 4 **LiquidMenu** objects and the number of the menu that will be shown first.



#### Menu example:
```c++
// This example demonstrates how to make a menu of 2 screens and 3 lines.

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// ---------- SCREEN 1 ----------
// Instantiate a LiquidLine object on column 4, row 0, printing "Hello".
LiquidLine line1(4, 0, "Hello");
LiquidLine line2(4, 1, "World");
// Put the above two lines into a LiquidScreen object.
LiquidScreen screen1(line1, line2);
// ------------------------------

// ---------- SCREEN 2 ----------
int variable1 = 123;
// Instantiate a line with text and variable.
LiquidLine var1_line(0, 0, "Variable 1: ", variable1);
// This screen will only have one line.
LiquidScreen screen2(var1_line);
// ------------------------------

// The menu object takes a reference to the LiquidCrystal object and the screens.
LiquidMenu menu(lcd, screen1, screen2);

```


#### Menu system example:
```c++
// This example demonstrates how to make a menu system of 3 menus, how to add
// a screen to a menu with a function and how to implement variable text.

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// Lines can be used in more than one screen.
LiquidLine back_line(10, 1, "-BACK");

// ---------- MAIN MENU ----------
// For changing text use char*.
char* name;
LiquidLine hello_line(4, 0, "Hello");
// This will print the text that the variable `name` points to.
LiquidLine name_line(4, 1, name);
LiquidScreen welcome_screen(hello_line, name_line);

// Lines can be used as links to other menus.
LiquidLine other_sLine(0, 0, "-Other");
LiquidLine preferences_sLine(0, 1, "-Preferences");
LiquidScreen shortcuts_screen(other_line, preferences_line);

LiquidMenu main_menu(lcd, welcome_screen, shortcuts_screen);
// -------------------------------

// ---------- OTHER MENU ----------
LiquidLine other_line(0, 0, "Other menu");
LiquidScreen other_screen(other_line, back_line);

LiquidMenu other_menu(lcd, other_screen);
// --------------------------------

// ---------- PREFERENCES MENU ----------
LiquidLine pref_line1(0, 0, "Prefs menu");
LiquidScreen pref_screen(pref_line1, back_line);

// The screen will be added later.
LiquidMenu pref_menu(lcd);
// --------------------------------------


// The menu system takes references to the menus.
LiquidSystem menu_system(main_menu, other_menu, pref_menu);


void setup() {
    // Change the variable 'name' so the 'welcome' screen prints "Hello Menu".
	name = "Menu";

    // Add a screen to a menu. The same works for the other object types.
	pref_menu.add_screen(pref_screen);
}

```


Navigating the menu
-------------------
The menu is navigated from the **LiquidMenu** object or if there are multiple menus - the **LiquidSystem** object.

#### Methods used:
```c++
void LiquidMenu::next_screen();
```
Go to the next screen.

```c++
void LiquidMenu::previous_screen();
```
Go to the previous screen.

```c++
bool LiquidMenu::change_screen(LiquidScreen &liquidScreen);
```
Go to a specific screen. Takes a **LiquidScreen** object reference.


#### Navigation example:
```c++
// This example demonstrates how to switch to the next/previous screen
// using some condition.

...
LiquidMenu menu(lcd, screen1, screen2);

void setup() {
    ...
	pinMode(rightButton, INPUT);
	pinMode(leftButton, INPUT);
}

void loop() {
    // Example condition that checks if a button is pressed.
    if (rightButton) {
        menu.next_screen();
    }
    if (leftButton) {
    	menu.previous_screen();
    }

}

```


Callback functions
------------------

### Attaching a function
Callback functions are attached to some **LiquidLine** object. Later they can be called on some event (*e.g. button press*), the function is identified by the **line** on which the focus is and by the number that is passed.

#### Methods used:
```c++
bool LiquidLine::attach_function(byte number, void (*function)(void));
```
Attaches a function to a **LiquidLine** object. Takes a number and a pointer to a function. The number is used later to identify the function when is is called.


#### Attaching a function example:
```c++
// This example demonstrates how to attach a function to some line. The
// callback function will change a variable and write its value to a
// pin. It will be attached to `led_line` under the number 1.

...
const byte ledPin = 3;
// We need a variable that holds the state.
byte ledState = 0;
LiquidLine led_line(0, 0, "LED: ", ledState);

/*
Callback functions for changing some state need to change the variable that
is printed on the display and in this case write in to the appropriate pin.
*/
void inc_led() {
	ledState += 10;
	analogWrite(ledPin, ledState);
}
...

void setup() {
    // The `void inc_led()` function is attached under number 1.
	led_line.attach_function(1, inc_led);
}

```

### Calling a function
To call an attached function it needs to be focused (selected) using the `void LiquidMenu::switch_focus()` function. When the function is selected, it can be called with `bool LiquidMenu::call_function(byte functionNumber)`.
Similar functions should be attached with the same number to the different lines. For example 'incrementing functions' can be attached with the number 1, then with a button press (*button 'UP' preferably*), the focused line's incrementing function will be called.

#### Methods used:
```c++
void LiquidMenu::switch_focus();
```
Switches the focus to the next **line**.

```c++
void LiquidMenu::call_function(byte functionNumber);
```
Calls the attached function for the currently focused line identified by `functionNumber`.


#### Calling a function example:
```c++
/// This example demonstrates how to call an attached function.
...
void loop() {
    // We need to be able to move the focus between the lines.
    if (middleButton) {
    	menu.switch_focus();
    }
    // If the 'UP' button is pressed - call the function
    // identified as 1 for the focused line.
    if (upButton) {
    	menu.call_function(1);
    }
}
```


Customizing the focus indicator
-------------------------------
The **focus indicator** on default appears just after the line.
It can be configured to appear before the line or in a custom absolute position.
If it appears after the line the symbol used is '<', before the line - '>', in a custom position - the symbol a square. These symbols can be changed individually.

#### Methods used:
```c++
bool LiquidLine::set_focusPosition(Position position, byte column = 0, byte row = 0);
```
Configures the focus position for the **line**. Takes a position from the **`Position`** enum class (*`LEFT`, `RIGHT` or `CUSTOM`*), if the specified position is `CUSTOM` then a column and row for the focus indicator need to be specified.

```c++
bool LiquidScreen::set_focusPosition(Position position);
```
Same as above, but configures a focus position for a whole **screen**. `CUSTOM` position can't be configured with this method because it needs individual column and row parameters.

```c++
bool LiquidMenu::set_focusPosition(Position position);
```
Same as above, but for a whole **menu**.

```c++
bool LiquidSystem::set_focusPosition(Position position);
```
Same as above, but for a whole **menu system**.

```c++
bool LiquidMenu::set_focusSymbol(Position position, byte symbol[8]);
```
Changes the focus symbol for the specified position.

```c++
bool LiquidMenu::set_focusSymbol(Position position, byte symbol[8])
```
Same as above (*because of the way the symbol is stored*).


#### Customizing the focus indicator example:
```c++
// This example demonstrates how to customize the focus indicator
// position and appearance.

...
// Indicator symbol definition. Create a new one from [here](http://omerk.github.io/lcdchargen/).
byte lFocus[8] = {
	0b00000,
	0b00000,
	0b00100,
	0b01110,
	0b00100,
	0b00000,
	0b00000,
	0b00000
};

void setup() {
    // Set the focus position for `line1` to `CUSTOM`.
	line1.set_focusPosition(Position::CUSTOM, 3, 0);
	// Set the focus position for `screen2` to `LEFT`.
	screen2.set_focusPosition(Position::LEFT);

    // Change the left focus indicator symbol to the specified one.
	menu.set_focusSymbol(Position::LEFT, lFocus);
}

```


More information
================
[Documentation](https://vasilkalchev.github.io/LiquidMenu/doc/html/index.html)

[More examples](https://vasilkalchev.github.io/LiquidMenu/doc/html/examples.html)



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

[lc]: https://github.com/arduino-libraries/LiquidCrystal