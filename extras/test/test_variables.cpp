/**
@file
What each supported variable type prints.

These are characterization tests: they record what the library does today,
type by type, so that any change to the storage mechanism has to reproduce
it exactly or say out loud that it did not. That makes them the safety net
for the planned move away from `DataType` + `recognizeType()`, where the
whole point is that only the behaviour marked KNOWN_BUG here should change.
*/

#include "lm_test.h"
#include <LiquidMenu.h>

namespace {

/// Builds a one line, one variable menu and returns what row 0 shows.
template <typename T>
std::string render(T &variable, uint8_t decimalPlaces = 2) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, variable);
  line.set_decimalPlaces(decimalPlaces);
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();
  return lcd.trimmedRow(0);
}

bool boolGetter() { return true; }
int8_t int8Getter() { return -12; }
uint8_t uint8Getter() { return 250; }
int16_t int16Getter() { return -1234; }
uint16_t uint16Getter() { return 60000; }
int32_t int32Getter() { return -123456; }
uint32_t uint32Getter() { return 4000000000u; }
float floatGetter() { return 1.5f; }
double doubleGetter() { return 2.25; }
char charGetter() { return 'G'; }
char *charPtrGetter() { static char text[] = "get"; return text; }
const char *constCharPtrGetter() { return "cget"; }

} //namespace

// ~ Text ~

TEST(string_literal_prints_the_string) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "literal");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();
  EQUAL(lcd.trimmedRow(0), "literal");
}

TEST(char_array_prints_the_string) {
  char text[] = "array";
  EQUAL(render(text), "array");
}

TEST(char_prints_one_character) {
  char character = 'X';
  EQUAL(render(character), "X");
}

// ~ Integers, at their limits ~

TEST(int8_prints_signed) {
  int8_t low = -128;
  int8_t high = 127;
  EQUAL(render(low), "-128");
  EQUAL(render(high), "127");
}

TEST(uint8_prints_unsigned) {
  uint8_t value = 255;
  EQUAL(render(value), "255");
}

TEST(int16_prints_signed) {
  int16_t low = -32768;
  EQUAL(render(low), "-32768");
}

TEST(uint16_prints_unsigned) {
  uint16_t value = 65535;
  EQUAL(render(value), "65535");
}

TEST(int32_prints_signed) {
  int32_t low = -2147483647 - 1;
  EQUAL(render(low), "-2147483648");
}

TEST(uint32_prints_unsigned) {
  uint32_t value = 4294967295u;
  EQUAL(render(value), "4294967295");
}

TEST(bool_prints_as_a_digit) {
  bool yes = true;
  bool no = false;
  EQUAL(render(yes), "1");
  EQUAL(render(no), "0");
}

// ~ Floating point ~

TEST(float_defaults_to_two_decimal_places) {
  float value = 3.14159f;
  EQUAL(render(value), "3.14");
}

TEST(float_honours_set_decimalPlaces) {
  float value = 3.14159f;
  EQUAL(render(value, 0), "3");
  EQUAL(render(value, 1), "3.1");
  EQUAL(render(value, 4), "3.1416");
}

TEST(double_honours_set_decimalPlaces) {
  double value = 2.5;
  EQUAL(render(value, 3), "2.500");
}

TEST(negative_float_keeps_its_sign) {
  float value = -0.75f;
  EQUAL(render(value, 2), "-0.75");
}

TEST(float_rounds_by_adding_a_half) {
  // Arduino's Print adds 0.5/10^digits and truncates. That is not the same
  // as printf("%.2f"), which gives "0.12" here - the fake deliberately
  // copies Print, because what the display shows is what matters.
  double value = 0.125;
  EQUAL(render(value, 2), "0.13");
}

// ~ Getter functions ~

TEST(getters_print_their_return_value) {
  EQUAL(render(boolGetter), "1");
  EQUAL(render(int8Getter), "-12");
  EQUAL(render(uint8Getter), "250");
  EQUAL(render(int16Getter), "-1234");
  EQUAL(render(uint16Getter), "60000");
  EQUAL(render(int32Getter), "-123456");
  EQUAL(render(uint32Getter), "4000000000");
  EQUAL(render(charGetter), "G");
  EQUAL(render(charPtrGetter), "get");
  EQUAL(render(constCharPtrGetter), "cget");
}

TEST(float_getter_ignores_set_decimalPlaces) {
  KNOWN_BUG(4, "FLOAT_GETTER/DOUBLE_GETTER do not pass _floatDecimalPlaces "
               "to print(), so the same value formats differently depending "
               "on whether it came from a variable or a getter");
  // A float variable holding 1.5 with 4 decimal places prints "1.5000";
  // the getter below prints the Print default instead.
  EQUAL(render(floatGetter, 4), "1.50");
  EQUAL(render(doubleGetter, 0), "2.25");
}

// ~ Bug 1, which cannot be exercised safely ~

/*
`add_variable()` stores `&variable` for every type, but the CHAR_PTR and
`*_GETTER` cases reinterpret the stored address *as* the value. On a device
that prints garbage; on the host it reads and calls through addresses that
are not ours, so these two cases crash the suite rather than failing it, and
the sanitizers flag them before they get that far.

They are kept here, off by default, as the executable description of the
bug. Build with `make unsafe` to watch them misbehave, and turn them into
ordinary passing assertions once the storage redesign lands.
*/
#if LM_TEST_UNSAFE_KNOWN_BUGS

TEST(char_pointer_variable_prints_garbage) {
  KNOWN_BUG(1, "a char* prints from the address of the pointer");
  char storage[] = "hi";
  char *pointer = storage;
  NOT_EQUAL(render(pointer), "hi");
}

TEST(const_char_pointer_variable_prints_garbage) {
  KNOWN_BUG(1, "the same for const char*, which is how most sketches would "
               "naturally hold a label - only a literal or an array passed "
               "straight to the constructor happens to work");
  const char *text = "hello";
  NOT_EQUAL(render(text), "hello");
}

TEST(getter_held_in_a_variable_is_not_called) {
  KNOWN_BUG(1, "a getter passed as a bare function name works, but one held "
               "in a function pointer variable is called through the address "
               "of the variable");
  int16_t (*getter)() = int16Getter;
  NOT_EQUAL(render(getter), "-1234");
}

#endif //LM_TEST_UNSAFE_KNOWN_BUGS
