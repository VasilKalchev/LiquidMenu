/**
@file
A fake "LiquidCrystal" library, used as `DisplayClass` in the host test
build - a third choice alongside `LiquidCrystal` and `LiquidCrystal_I2C`,
hence the name.

It offers the same call surface LiquidMenu uses from a real one (`print`,
`write`, `createChar`, `clear`, `setCursor`) but renders into a character
buffer the tests can read back, so an assertion can be written against what
the display would actually show.

The `print()` overloads and the number/float formatting are copied from
Arduino's `Print` class on purpose. Formatting differences are exactly the
kind of regression these tests exist to catch, so the fake must not invent
its own - notably `printFloat()` rounds by adding 0.5/10^digits and
truncating, which does not always agree with `printf("%.*f")`.

@see extras/test/README.md
*/

#pragma once

#include <stdint.h>
#include <math.h>
#include <string>
#include <vector>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

/// A character display that renders into a buffer instead of onto glass.
class LiquidCrystal_fake {
public:
  LiquidCrystal_fake(uint8_t columns = 16, uint8_t rows = 2)
    : _columns(columns), _rows(rows) {
    clear();
    for (uint8_t i = 0; i < 8; i++) {
      _glyphCreated[i] = false;
    }
  }

  // ~ The "LiquidCrystal" surface LiquidMenu calls ~

  void clear() {
    _buffer.assign(_rows, std::string(_columns, ' '));
    _column = 0;
    _row = 0;
    _clears++;
    _overflows = 0;
  }

  void setCursor(uint8_t column, uint8_t row) {
    _column = column;
    _row = row;
  }

  /// Custom characters are stored by index; indices 13-15 are the focus symbols.
  void createChar(uint8_t index, uint8_t character[8]) {
    if (index < 8) {
      for (uint8_t i = 0; i < 8; i++) {
        _glyph[index][i] = character[i];
      }
      _glyphCreated[index] = true;
    }
    _createCharCalls++;
    _lastCreateCharIndex = index;
  }

  size_t write(uint8_t character) {
    put((char)character);
    return 1;
  }

  // ~ Arduino `Print`'s overload set ~

  size_t print(const char str[]) {
    size_t n = 0;
    while (*str) {
      put(*str++);
      n++;
    }
    return n;
  }
  size_t print(char *str) { return print((const char *)str); }
  size_t print(char character) { put(character); return 1; }
  size_t print(unsigned char value, int base = DEC) { return printNumber(value, base); }
  size_t print(short value, int base = DEC) { return printSigned(value, base); }
  size_t print(unsigned short value, int base = DEC) { return printNumber(value, base); }
  size_t print(int value, int base = DEC) { return printSigned(value, base); }
  size_t print(unsigned int value, int base = DEC) { return printNumber(value, base); }
  size_t print(long value, int base = DEC) { return printSigned((int32_t)value, base); }
  size_t print(unsigned long value, int base = DEC) { return printNumber((uint32_t)value, base); }
  size_t print(double value, int digits = 2) { return printFloat(value, (uint8_t)digits); }

  // ~ What the tests read back ~

  /// The contents of one row, padded with spaces to the display width.
  std::string row(uint8_t row) const { return _buffer[row]; }

  /// Every row, newline separated, trailing spaces kept.
  std::string screen() const {
    std::string screen;
    for (size_t r = 0; r < _buffer.size(); r++) {
      screen += _buffer[r];
      screen += '\n';
    }
    return screen;
  }

  /// One row with trailing spaces removed, for terser assertions.
  std::string trimmedRow(uint8_t row) const {
    std::string line = _buffer[row];
    size_t end = line.find_last_not_of(' ');
    return end == std::string::npos ? std::string() : line.substr(0, end + 1);
  }

  uint8_t columns() const { return _columns; }
  uint8_t rows() const { return _rows; }
  uint8_t cursorColumn() const { return _column; }
  uint8_t cursorRow() const { return _row; }

  /// Characters that fell outside the display since the last clear().
  unsigned overflows() const { return _overflows; }
  unsigned clears() const { return _clears; }
  unsigned createCharCalls() const { return _createCharCalls; }
  uint8_t lastCreateCharIndex() const { return _lastCreateCharIndex; }
  bool glyphCreated(uint8_t index) const { return index < 8 ? _glyphCreated[index] : false; }
  uint8_t glyphRow(uint8_t index, uint8_t row) const { return _glyph[index][row]; }

private:
  /// A real HD44780 does not wrap; anything off the display is dropped.
  void put(char character) {
    if (_row < _rows && _column < _columns) {
      _buffer[_row][_column] = character;
    } else {
      _overflows++;
    }
    _column++;
  }

  // Print::printNumber(), with AVR's 32 bit `unsigned long`.
  size_t printNumber(uint32_t value, int base) {
    char buffer[8 * sizeof(uint32_t) + 1];
    char *str = &buffer[sizeof(buffer) - 1];
    *str = '\0';
    if (base < 2) {
      base = 10;
    }
    do {
      char digit = (char)(value % (uint32_t)base);
      value /= (uint32_t)base;
      *--str = digit < 10 ? (char)(digit + '0') : (char)(digit + 'A' - 10);
    } while (value);
    return print((const char *)str);
  }

  size_t printSigned(int32_t value, int base) {
    if (base == 10 && value < 0) {
      size_t n = print('-');
      // Negated in unsigned arithmetic so that INT32_MIN stays defined.
      return n + printNumber((uint32_t)0 - (uint32_t)value, 10);
    }
    return printNumber((uint32_t)value, base);
  }

  // Print::printFloat(), rounding included.
  size_t printFloat(double value, uint8_t digits) {
    if (isnan(value)) { return print("nan"); }
    if (isinf(value)) { return print("inf"); }
    if (value > 4294967040.0 || value < -4294967040.0) { return print("ovf"); }

    size_t n = 0;
    if (value < 0.0) {
      n += print('-');
      value = -value;
    }

    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; i++) {
      rounding /= 10.0;
    }
    value += rounding;

    uint32_t integerPart = (uint32_t)value;
    double remainder = value - (double)integerPart;
    n += printNumber(integerPart, 10);

    if (digits > 0) {
      n += print('.');
    }
    while (digits-- > 0) {
      remainder *= 10.0;
      unsigned int digit = (unsigned int)remainder;
      n += printNumber(digit, 10);
      remainder -= digit;
    }
    return n;
  }

  uint8_t _columns, _rows;
  uint8_t _column = 0, _row = 0;
  std::vector<std::string> _buffer;
  uint8_t _glyph[8][8] = {};
  bool _glyphCreated[8];
  unsigned _overflows = 0, _clears = 0, _createCharCalls = 0;
  uint8_t _lastCreateCharIndex = 0;
};
