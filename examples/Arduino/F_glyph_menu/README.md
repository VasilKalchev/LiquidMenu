# F - glyph_menu

Printing custom characters (glyphs) in a menu and animating one of them.

**Demonstrates:** `LiquidCrystal::createChar()` together with
`LiquidLine::set_asGlyph()`, animating a glyph by changing the index the line
points to, `LiquidMenu::softUpdate()` for a redraw without flicker.

**Hardware:** Arduino Uno/Nano, HD44780 16x2 display, 10k potentiometer for the
contrast.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
