# D - encoder_menu

Navigating and editing the menu with a single rotary encoder, including the
"edit mode" pattern that a one-knob interface needs.

**Demonstrates:** driving the menu from an encoder, an edit mode (rotation
either moves the focus or changes the focused value), `LiquidMenu::is_callable()`
to tell what the focused line can do, `LiquidMenu::set_focusSymbol()` used as a
mode indicator, using the focus "ghosting" step to change the screen.

**Hardware:** Arduino Uno/Nano, HD44780 16x2 display, 10k potentiometer for the
contrast, a KY-040 style rotary encoder (CLK 7, DT 8, SW 9) and an LED with a
resistor on pin 6.

**Try it:** turn the knob to move the focus, press it on a value to start
editing, turn to change the value, press again to stop.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
