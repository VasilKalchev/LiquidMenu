# D - system_menu (ESP32)

Sub-menus with `LiquidSystem`, with the settings kept in the ESP32's NVS flash
storage.

**Demonstrates:** `LiquidSystem` and `change_menu()` as a callback, reusing a
`LiquidLine` across menus, and the `Preferences` library as the ESP32's
replacement for the EEPROM.

**Requires:** the I2C display library configuration - see
[`B_I2C_menu`](../B_I2C_menu).

**Hardware:** ESP32 DevKit, I2C display (SDA 21, SCL 22), five push-buttons to
ground (LEFT 32, RIGHT 33, UP 25, DOWN 26, ENTER 27), an LED with a resistor on
GPIO 4 and a potentiometer on GPIO 35.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
