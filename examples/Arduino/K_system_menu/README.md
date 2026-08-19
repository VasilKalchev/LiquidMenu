# K - system_menu

The largest example: one menu split into sub-menus, with the settings stored in
the EEPROM.

**Demonstrates:** `LiquidSystem`, `LiquidSystem::change_menu()` wrapped in a
callback to build "/Outputs" and "/BACK" entries, reusing a `LiquidLine` across
menus, persisting settings.

**Hardware:** Arduino Uno/Nano, HD44780 16x2 display, 10k potentiometer for the
contrast, five push-buttons to ground (LEFT A0, RIGHT 7, UP 8, DOWN 9,
ENTER 10), an LED with a resistor on pin 6 and a potentiometer on A1.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
