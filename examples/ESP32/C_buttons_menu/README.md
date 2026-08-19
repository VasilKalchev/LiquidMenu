# C - buttons_menu (ESP32)

The interactive menu on an ESP32: five buttons navigate, focus and call
functions that dim an LED.

**Demonstrates:** `attach_function()` / `call_function()` / `switch_focus()` on
an ESP32, plus the platform details that bite: `INPUT_PULLUP` not being
available on GPIO 34-39, and PWM through `analogWrite()` (core 3.x) or the LEDC
API (core 2.x).

**Requires:** the I2C display library configuration - see
[`B_I2C_menu`](../B_I2C_menu).

**Hardware:** ESP32 DevKit, I2C display (SDA 21, SCL 22), five push-buttons to
ground (LEFT 32, RIGHT 33, UP 25, DOWN 26, ENTER 27) and an LED with a resistor
on GPIO 4.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
