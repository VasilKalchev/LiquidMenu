# I - progmem_menu

Keeping menu texts in flash memory instead of RAM - an AVR technique for when
the 2KB of RAM starts running out.

**Demonstrates:** `PROGMEM` strings in a `LiquidLine` together with
`LiquidLine::set_asProgmem()`.

**Hardware:** Arduino Nano (the pin numbers are the same on an Uno), HD44780
16x2 display, 10k potentiometer for the contrast.

**Note:** AVR only. On the ESP32, RP2040 or STM32 the `PROGMEM` macro exists
but does nothing - there the strings are addressable directly and this
technique is unnecessary.

**Simulate:** [`diagram.json`](diagram.json) - see
[Simulating the examples](../../README.md#simulating-the-examples-with-wokwi).
