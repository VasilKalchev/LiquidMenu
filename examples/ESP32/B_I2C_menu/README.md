# B - I2C_menu (ESP32)

The usual ESP32 setup: an I2C ("backpack") display on two wires.

**Demonstrates:** configuring the library for an I2C display library,
`LiquidMenu::init()`, and choosing the I2C pins with `Wire.begin(SDA, SCL)`.

**Requires:** the library has to be pointed at the I2C display library in
[`src/LiquidMenu_config.h`](../../../src/LiquidMenu_config.h):

```c++
#define LIQUIDMENU_LIBRARY LiquidCrystal_I2C_LIBRARY
#define DisplayClass LiquidCrystal_I2C
```

Tested with [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C).

**Hardware:** ESP32 DevKit, HD44780 16x2 display with an I2C backpack
(SDA 21, SCL 22), a potentiometer on GPIO 34.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
