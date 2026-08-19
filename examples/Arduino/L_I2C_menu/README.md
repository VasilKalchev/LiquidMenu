# L - I2C_menu

The same menu as "A_hello_menu", but on an I2C ("backpack") display.

**Demonstrates:** configuring the library for an I2C display library and
`LiquidMenu::init()`, which is required when the display is initialized from
`setup()` rather than from its constructor.

**Requires:** the library has to be pointed at the I2C display library in
[`src/LiquidMenu_config.h`](../../../src/LiquidMenu_config.h):

```c++
#define LIQUIDMENU_LIBRARY LiquidCrystal_I2C_LIBRARY
#define DisplayClass LiquidCrystal_I2C
```

Tested with [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C).

**Hardware:** Arduino Uno/Nano, HD44780 16x2 display with an I2C backpack
(SDA A4, SCL A5), a potentiometer on A1.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
