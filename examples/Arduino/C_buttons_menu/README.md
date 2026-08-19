# C - buttons_menu

The complete interactive menu: five buttons navigate the screens, move the
focus and call functions that dim an LED and toggle a blink loop.

**Demonstrates:** `LiquidLine::attach_function()`, `LiquidMenu::call_function()`,
`LiquidMenu::switch_focus()`, grouping similar callbacks under one number,
printing text that changes at runtime (`char` array).

**Hardware:** Arduino Uno/Nano, HD44780 16x2 display, 10k potentiometer for the
contrast, five push-buttons to ground (LEFT A0, RIGHT 7, UP 8, DOWN 9,
ENTER 10) and an LED with a resistor on pin 6.

**Try it:** RIGHT/LEFT change the screen, ENTER focuses a line, UP/DOWN act on
the focused line.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
