# LiquidMenu examples

The examples are grouped by board and ordered by complexity - `A` is the one to
start with, and each following letter assumes what the previous ones showed.
The Arduino set is the complete tour of the library; the ESP32 set covers the
same ground in fewer sketches and concentrates on what is different on that
platform.

Every example folder contains the sketch, a short `README.md` and a
[`diagram.json`](#simulating-the-examples-with-wokwi) that runs it in a
simulator, no hardware required.

## Arduino (Uno, Nano and other AVR boards)

| # | Example | What it adds |
|---|---------|--------------|
| A | [hello_menu](Arduino/A_hello_menu) | lines, screens, a menu, printing a live variable |
| B | [serial_menu](Arduino/B_serial_menu) | every navigation method, driven from the Serial Monitor |
| C | [buttons_menu](Arduino/C_buttons_menu) | buttons, callback functions, focus, changing text |
| D | [encoder_menu](Arduino/D_encoder_menu) | a rotary encoder and an edit mode |
| E | [focus_menu](Arduino/E_focus_menu) | where the focus indicator is and what it looks like |
| F | [glyph_menu](Arduino/F_glyph_menu) | custom characters and animating them |
| G | [scrolling_menu](Arduino/G_scrolling_menu) | more lines than the display has; decimal places |
| H | [getters_menu](Arduino/H_getters_menu) | getter functions instead of variables |
| I | [progmem_menu](Arduino/I_progmem_menu) | keeping texts in flash instead of RAM (AVR) |
| J | [dynamic_menu](Arduino/J_dynamic_menu) | hiding and showing screens at runtime |
| K | [system_menu](Arduino/K_system_menu) | sub-menus with `LiquidSystem`; saved settings |
| L | [I2C_menu](Arduino/L_I2C_menu) | using an I2C ("backpack") display |

## ESP32

| # | Example | What it adds |
|---|---------|--------------|
| A | [hello_menu](ESP32/A_hello_menu) | the basics, and what differs from an AVR board |
| B | [I2C_menu](ESP32/B_I2C_menu) | an I2C display, the usual ESP32 wiring |
| C | [buttons_menu](ESP32/C_buttons_menu) | buttons and callbacks; pull-ups and PWM on the ESP32 |
| D | [system_menu](ESP32/D_system_menu) | sub-menus; settings in NVS with `Preferences` |

## Using an I2C display

The wrapped display library is a compile time choice, so an I2C display needs
one edit in [`src/LiquidMenu_config.h`](../src/LiquidMenu_config.h) - comment
out the parallel block and uncomment the I2C one:

```c++
#define LIQUIDMENU_LIBRARY LiquidCrystal_I2C_LIBRARY
#define DisplayClass LiquidCrystal_I2C
```

The examples that need this say so at the top. The other difference is that
`menu.init()` has to be called in `setup()` after the display library's own
initialization.

## Simulating the examples with Wokwi

Each example folder has a `diagram.json` - a [Wokwi](https://wokwi.com)
circuit with the board, the display and the inputs already wired to the pins
the sketch expects.

**In the browser:**

1. Create a new project for the right board on [wokwi.com](https://wokwi.com).
2. Open the `diagram.json` tab and replace its content with this folder's file.
3. Paste the sketch into `sketch.ino`.
4. In the Library Manager tab add `LiquidMenu` (and `LiquidCrystal_I2C` for the
   I2C examples), then press play.

**In VS Code:** with the
[Wokwi extension](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode)
installed, add a `wokwi.toml` next to the `diagram.json` pointing at the build
output of your Arduino CLI or PlatformIO build, then run "Wokwi: Start
Simulator".

The simulated display ignores the contrast potentiometer, but it is kept in the
diagrams because a real one needs it.

## Running on other boards

The library itself is board independent - it only calls into the display
library - so the sketches port with little more than a change of pin numbers.
What to look out for:

- **Pin names.** `A0`-style aliases and `LED_BUILTIN` are not defined
  everywhere. On the ESP32 use plain GPIO numbers, and check which pins are
  input only or reserved for booting.
- **`PROGMEM`.** Only meaningful on AVR. Elsewhere the macro compiles to
  nothing and `set_asProgmem()` is unnecessary - skip
  [`I_progmem_menu`](Arduino/I_progmem_menu).
- **The EEPROM.** ESP32 and RP2040 have no real EEPROM; their cores emulate one
  in flash, and each has a native alternative (`Preferences` on the ESP32,
  `LittleFS` on the RP2040). STM32 boards vary - some emulate the EEPROM, some
  have a small battery-backed area.
- **PWM.** `analogWrite()` exists on the ESP32 only from core 3.x (use the LEDC
  API on 2.x), and its resolution differs between platforms.
- **Integer types.** Prefer the fixed width types (`uint8_t`, `uint16_t`...)
  for the variables put in a `LiquidLine`. On a 32-bit core a plain
  `unsigned int` is ambiguous for the library's type recognition and fails to
  compile.
- **Logic levels.** 3.3V boards drive most HD44780 modules fine while the module
  is powered from 5V, but a stubborn display may need a level shifter or 3.3V
  power.
- **ADC range.** `analogRead()` returns 0..1023 on AVR, 0..4095 on the ESP32
  and 0..1023 (12 bit hardware, 10 bit by default) on the RP2040.

STM32, RP2040 and SAMD boards are otherwise a straight fit: take the Arduino
examples, change the LCD and button pins at the top of the sketch and they
compile and run as they are.
