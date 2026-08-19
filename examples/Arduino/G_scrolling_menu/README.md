# G - scrolling_menu

A screen with more lines than the display has - the extra ones scroll into view
as the focus moves.

**Demonstrates:** `LiquidScreen::set_displayLineCount()`, why every scrolled
line needs an attached function, `LiquidLine::set_decimalPlaces()`.

**Hardware:** Arduino Nano (the pin numbers are the same on an Uno), HD44780
16x2 display, 10k potentiometer for the contrast, one push-button to ground
(ENTER 10).

**Try it:** press ENTER repeatedly and watch the five lines scroll through the
two-line display.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
