# J - dynamic_menu

A menu that changes while it runs: an "advanced" section that only appears once
it is switched on.

**Demonstrates:** `LiquidScreen::hide()`, `LiquidMenu::change_screen()` for
jumping straight to a screen, `LiquidMenu::get_currentScreen()` for finding out
where the menu is.

**Hardware:** Arduino Uno/Nano, HD44780 16x2 display, 10k potentiometer for the
contrast, five push-buttons to ground (LEFT A0, RIGHT 7, UP 8, DOWN 9,
ENTER 10).

**Try it:** on the "Advanced" screen press ENTER to focus the line and UP to
turn it on - two more screens join the rotation.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
