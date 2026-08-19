# A - hello_menu (ESP32)

The "get started" example for the ESP32: two screens, the second one showing a
live analog reading.

**Demonstrates:** `LiquidLine`, `LiquidScreen` and `LiquidMenu` on an ESP32,
plus what differs from an AVR board: GPIO numbers instead of `A1`-style
aliases, which pins are safe to use, 12-bit `analogRead()` and the 3.3V logic.

**Hardware:** ESP32 DevKit, HD44780 16x2 display in 4-bit parallel mode
(RS 13, E 14, D4 27, D5 26, D6 25, D7 33), 10k potentiometer for the contrast
and a second one on GPIO 34.

**Note:** wiring six data lines is unusual on an ESP32 - see
[`B_I2C_menu`](../B_I2C_menu) for the two-wire version.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
