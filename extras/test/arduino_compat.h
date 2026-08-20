/**
@file
The few Arduino/AVR facilities LiquidMenu uses that the host does not have.

`src/LiquidMenu.h` includes `<avr/pgmspace.h>` only `#if defined(__AVR__)`,
and other cores supply their own emulation of it. The host build needs the
same, so this provides the three names `LiquidLine::print_variable()` uses
for `DataType::PROG_CONST_CHAR_PTR`. Flash and RAM are the same memory here,
so reading "from flash" is an ordinary dereference - the same simplification
the ESP32 core makes.

Force-included ahead of everything else by the Makefile.
*/

#pragma once

#include <string.h>

#ifndef PROGMEM
# define PROGMEM
#endif
#ifndef PSTR
# define PSTR(str) (str)
#endif
#ifndef pgm_read_byte_near
# define pgm_read_byte_near(address) (*(const unsigned char *)(address))
#endif
#ifndef pgm_read_byte
# define pgm_read_byte(address) pgm_read_byte_near(address)
#endif
#ifndef strlen_P
# define strlen_P strlen
#endif
