/**
@file
LiquidLine: placement, the variable slots, the focus indicator, callbacks.
*/

#include "lm_test.h"
#include <LiquidMenu.h>

namespace {

int8_t one = 1, two = 2, three = 3, four = 4, five = 5, six = 6;

unsigned callCount = 0;
void countingCallback() { callCount++; }

const char progmemText[] PROGMEM = "flash";
const char longProgmemText[] PROGMEM = "a rather longer string than the display";
uint8_t glyphIndex = 3;

} //namespace

TEST(a_line_prints_at_its_column_and_row) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(4, 1, "here");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();

  EQUAL(lcd.row(0), std::string(16, ' '));
  EQUAL(lcd.row(1), "    here        ");
}

TEST(the_constructor_takes_up_to_four_variables) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, one, two, three, four);
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();

  EQUAL(lcd.trimmedRow(0), "1234");
}

TEST(add_variable_appends_and_stops_at_MAX_VARIABLES) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, one, two, three, four);

  TRUE(line.add_variable(five));   // the fifth slot, MAX_VARIABLES is 5
  FALSE(line.add_variable(six));   // rejected, and not printed

  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();
  EQUAL(lcd.trimmedRow(0), "12345");
}

TEST(text_past_the_last_column_is_dropped_not_wrapped) {
  LiquidCrystal_fake lcd(8, 2);
  LiquidLine line(4, 0, "abcdefgh");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();

  EQUAL(lcd.row(0), "    abcd");
  EQUAL(lcd.row(1), "        ");
  EQUAL(lcd.overflows(), 4u);
}

// ~ Focus ~
//
// A line only becomes focusable by having a function attached to it, and a
// freshly built screen starts with the focus parked past the last line, so
// nothing is focused until switch_focus() is called. Both of those surprise
// people, so they are pinned down first.

TEST(nothing_is_focused_until_switch_focus_is_called) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  line.attach_function(1, countingCallback);
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();

  EQUAL(lcd.trimmedRow(0), "text");
  EQUAL(menu.get_focusedLine(), 1); // one past the only line
}

TEST(a_line_without_a_function_can_never_be_focused) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  EQUAL(lcd.trimmedRow(0), "text");
  FALSE(menu.set_focusedLine(0));
}

TEST(the_focused_line_gets_the_right_focus_symbol_by_default) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  line.attach_function(1, countingCallback);
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  // Position::NORMAL and Position::RIGHT are the same enumerator, so the
  // symbol (custom character 15) follows the text on the focused line.
  EQUAL(lcd.trimmedRow(0), std::string("text") + (char)15);
}

TEST(focus_position_left_puts_the_symbol_before_the_line) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(4, 0, "text");
  line.attach_function(1, countingCallback);
  TRUE(line.set_focusPosition(Position::LEFT));
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  EQUAL(lcd.row(0), std::string("   ") + (char)14 + "text        ");
}

TEST(focus_position_custom_puts_the_symbol_where_asked) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  line.attach_function(1, countingCallback);
  TRUE(line.set_focusPosition(Position::CUSTOM, 10, 1));
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  EQUAL(lcd.trimmedRow(0), "text");
  EQUAL(lcd.row(1), std::string(10, ' ') + (char)13 + "     ");
}

TEST(an_unfocused_line_gets_no_symbol) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine first(0, 0, "one");
  LiquidLine second(0, 1, "two");
  first.attach_function(1, countingCallback);
  LiquidScreen screen(first, second);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  EQUAL(lcd.trimmedRow(0), std::string("one") + (char)15);
  EQUAL(lcd.trimmedRow(1), "two");
}

// ~ Callbacks ~

TEST(attach_function_makes_a_line_callable) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "call");
  TRUE(line.attach_function(1, countingCallback));

  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  TRUE(menu.is_callable(1));
  FALSE(menu.is_callable(2));

  unsigned before = callCount;
  TRUE(menu.call_function(1));
  EQUAL(callCount, before + 1);

  FALSE(menu.call_function(2));
  EQUAL(callCount, before + 1);
}

TEST(attach_function_rejects_a_number_outside_the_slots) {
  LiquidLine line(0, 0, "call");

  FALSE(line.attach_function(MAX_FUNCTIONS + 1, countingCallback));

  // Functions are numbered from 1. Number 0 used to pass the upper-bound
  // check and then write to _function[255].
  FALSE(line.attach_function(0, countingCallback));
}

TEST(is_callable_and_call_function_reject_a_number_outside_the_slots) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "call");
  line.attach_function(1, countingCallback);
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();

  unsigned before = callCount;

  FALSE(menu.is_callable(0));
  FALSE(menu.call_function(0));
  FALSE(menu.is_callable(MAX_FUNCTIONS + 1));
  FALSE(menu.call_function(MAX_FUNCTIONS + 1));

  EQUAL(callCount, before);
}

TEST(a_line_without_a_function_is_not_callable) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "plain");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  FALSE(menu.is_callable(1));
  FALSE(menu.call_function(1));
}

// ~ The two variable modifiers ~

TEST(set_asGlyph_writes_the_index_as_a_character) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "g:", glyphIndex);
  TRUE(line.set_asGlyph(2));
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();

  // Without set_asGlyph the uint8_t would have printed as "3".
  EQUAL(lcd.trimmedRow(0), std::string("g:") + (char)3);
}

TEST(set_asGlyph_rejects_a_slot_that_is_not_a_byte) {
  LiquidLine line(0, 0, "text");
  FALSE(line.set_asGlyph(1));
}

TEST(set_asProgmem_reads_the_string_from_flash) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, progmemText);
  TRUE(line.set_asProgmem(1));
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();

  EQUAL(lcd.trimmedRow(0), "flash");
}

TEST(set_asProgmem_handles_a_string_longer_than_the_display) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, longProgmemText);
  TRUE(line.set_asProgmem(1));
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.update();

  EQUAL(lcd.row(0), "a rather longer ");
  EQUAL(lcd.overflows(), 23u);
}

TEST(set_asProgmem_rejects_a_slot_that_is_not_a_string) {
  LiquidLine line(0, 0, one);
  FALSE(line.set_asProgmem(1));
}
