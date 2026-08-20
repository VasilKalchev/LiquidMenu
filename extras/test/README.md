# Tests

A test suite that runs on your computer. There is no board involved, no
Arduino installation and nothing to install first:

```sh
cd extras/test
make
```

It compiles the library for the host and runs every case, printing a line
per case and a failure per broken assertion:

```
59 cases, 205 checks, 0 cases failed, 0 checks failed
```

A non-zero exit status means something broke. That is the whole purpose:
**to notice when a change breaks something that used to work.**

## How the library can run without a display

LiquidMenu reaches a "LiquidCrystal" library through exactly one name, the
`DisplayClass` macro in [`LiquidMenu_config.h`](../../src/LiquidMenu_config.h).
The test build points that macro at
[`LiquidCrystal_fake`](LiquidCrystal_fake.h) instead - a third choice
alongside `LiquidCrystal` and `LiquidCrystal_I2C`, with the same `print` /
`write` / `createChar` / `clear` / `setCursor` surface, rendering into a
character buffer. A test can then assert on what
the display would show:

```cpp
LiquidCrystal_fake lcd(16, 2);
uint16_t count = 7;
LiquidLine line(0, 0, "N: ", count);
LiquidScreen screen(line);
LiquidMenu menu(lcd, screen);
menu.update();

EQUAL(lcd.trimmedRow(0), "N: 7");
```

`LIQUIDMENU_LIBRARY` is set to `Mock_LIBRARY` so that no real display header
is included, and [`arduino_compat.h`](arduino_compat.h) supplies the handful
of AVR flash macros (`PROGMEM`, `strlen_P`, `pgm_read_byte_near`) that the
host does not have - the same emulation other cores provide.

`LiquidCrystal_fake`'s `print()` overloads and its number and float formatting are
copied from Arduino's `Print` class rather than written from scratch. That
matters: `Print` rounds a float by adding 0.5/10^digits and truncating,
which does not always agree with `printf("%.*f")`. A fake that formatted
differently would quietly hide exactly the kind of change these tests exist
to catch.

Two deliberate simplifications: characters written past the last column are
dropped and counted by `overflows()` rather than landing wherever an
HD44780's address counter would put them, and custom characters are recorded
by index without being rendered.

## Writing a test

Add a `TEST(name) { ... }` to one of the `test_*.cpp` files, or to a new one
- the Makefile picks up every `.cpp` in this folder, and cases register
themselves. `EQUAL`, `NOT_EQUAL`, `TRUE` and `FALSE` record a failure and
carry on, so one broken case reports all of its problems at once.

Prefer asserting on what the display shows over asserting on internal state.
It is what users experience, and it survives a rewrite of the internals.

## What is covered

| File | Subject |
| --- | --- |
| [`test_variables.cpp`](test_variables.cpp) | what every supported variable type and getter prints |
| [`test_liquidline.cpp`](test_liquidline.cpp) | placement, variable slots, focus indicator, callbacks, glyphs, PROGMEM |
| [`test_liquidscreen.cpp`](test_liquidscreen.cpp) | line ordering, focus cycling and ghosting, hiding, scrolling |
| [`test_liquidmenu.cpp`](test_liquidmenu.cpp) | screens, navigation, operators, what reaches the display |
| [`test_liquidsystem.cpp`](test_liquidsystem.cpp) | several menus, and delegation to the current one |

`test_variables.cpp` is the important one for any change to how a
`LiquidLine` stores its variables. It records today's output type by type,
so a rewrite of the storage has to reproduce all of it or say out loud what
it changed.

## Known bugs, on purpose

Some cases pin down behaviour that is *wrong*, marked with `KNOWN_BUG`.
Fixing the bug is supposed to make them fail - that is how you find the
assertions to rewrite. Do not "fix" a `KNOWN_BUG` case by loosening it.

A few bugs cannot be run at all. `add_variable()` stores `&variable` for
every type, but the `char*`, `const char*` and getter cases reinterpret that
address *as* the value, so on the host they read and call through addresses
that are not ours. Those cases live behind `LM_TEST_UNSAFE_KNOWN_BUGS` at
the bottom of `test_variables.cpp`; `make unsafe` builds them, and they
crash rather than fail.

## Sanitizers

```sh
make asan
```

rebuilds with `-fsanitize=address,undefined`. Most of this library's open
bugs are memory bugs - a buffer written one past its end, an index that is
not bounds checked - and the host is the only place they can be caught
automatically rather than by reading the code. This needs the sanitizer
runtimes installed (`libasan`, `libubsan`); it is expected to report the
known bounds bugs until they are fixed.

## In CI

[`.github/workflows/test.yml`](../../.github/workflows/test.yml) runs `make`
on every push, on every branch, and on every pull request - so a change
gets checked while you are still working on it, and again when it is merged.
It takes seconds and needs no board, so unlike the compile and lint jobs it
can actually tell you whether the library still *works*.
