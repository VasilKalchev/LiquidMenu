/**
@file
LiquidMenu: holding screens, moving between them, and the display calls it
makes while doing so.
*/

#include "lm_test.h"
#include <LiquidMenu.h>

namespace {

void noop() {}

unsigned callCount = 0;
void countingCallback() { callCount++; }

uint8_t customSymbol[8] = {0, 1, 2, 3, 4, 5, 6, 7};

} //namespace

TEST(add_screen_stops_at_MAX_SCREENS) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidMenu menu(lcd);
  LiquidScreen screen;

  for (uint8_t i = 0; i < MAX_SCREENS; i++) {
    TRUE(menu.add_screen(screen));
  }
  FALSE(menu.add_screen(screen));
}

TEST(the_starting_screen_can_be_chosen) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second, 2);
  menu.update();

  EQUAL(lcd.trimmedRow(0), "second");
}

TEST(next_and_previous_screen_wrap_around) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);

  menu.update();
  EQUAL(lcd.trimmedRow(0), "first");

  menu.next_screen();
  EQUAL(lcd.trimmedRow(0), "second");
  menu.next_screen();
  EQUAL(lcd.trimmedRow(0), "first");

  menu.previous_screen();
  EQUAL(lcd.trimmedRow(0), "second");
}

TEST(the_increment_and_decrement_operators_change_screens) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);
  menu.update();

  ++menu;
  EQUAL(lcd.trimmedRow(0), "second");
  --menu;
  EQUAL(lcd.trimmedRow(0), "first");
  menu++;
  EQUAL(lcd.trimmedRow(0), "second");
  menu--;
  EQUAL(lcd.trimmedRow(0), "first");
}

TEST(change_screen_by_number_is_one_based) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);

  TRUE(menu.change_screen(2));
  EQUAL(lcd.trimmedRow(0), "second");
  TRUE(menu.change_screen(1));
  EQUAL(lcd.trimmedRow(0), "first");

  // Screen 0 does not exist; the index underflows and is rejected.
  FALSE(menu.change_screen((uint8_t)0));
  EQUAL(lcd.trimmedRow(0), "first");
}

TEST(change_screen_rejects_a_number_past_the_last_screen) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);
  menu.update();

  // The bound used to be `index <= _screenCount`, so this was accepted and
  // the update that followed read past the end of an array that is never
  // initialised.
  FALSE(menu.change_screen(3));
  EQUAL(menu.get_currentScreen(), &first);

  FALSE(menu.change_screen(200));
  EQUAL(menu.get_currentScreen(), &first);
}

TEST(change_screen_by_pointer_finds_the_screen) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidScreen stranger;
  LiquidMenu menu(lcd, first, second);

  TRUE(menu.change_screen(&second));
  EQUAL(lcd.trimmedRow(0), "second");
  EQUAL(menu.get_currentScreen(), &second);

  FALSE(menu.change_screen(&stranger));
  EQUAL(menu.get_currentScreen(), &second);
}

TEST(the_assignment_operator_changes_screens_too) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);

  TRUE(menu = &second);
  EQUAL(lcd.trimmedRow(0), "second");
  TRUE(menu = (uint8_t)1);
  EQUAL(lcd.trimmedRow(0), "first");
}

// ~ What reaches the display ~

TEST(update_clears_the_display_but_softUpdate_does_not) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);

  unsigned clearsBefore = lcd.clears();
  menu.update();
  EQUAL(lcd.clears(), clearsBefore + 1);

  menu.softUpdate();
  EQUAL(lcd.clears(), clearsBefore + 1);
  EQUAL(lcd.trimmedRow(0), "text");
}

TEST(init_creates_the_three_focus_symbols) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);

  unsigned before = lcd.createCharCalls();
  menu.init();
  EQUAL(lcd.createCharCalls(), before + 3);
  EQUAL(lcd.lastCreateCharIndex(), 13); // right, left, then custom
}

TEST(set_focusSymbol_replaces_one_of_them) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);

  TRUE(menu.set_focusSymbol(Position::RIGHT, customSymbol));
  EQUAL(lcd.lastCreateCharIndex(), 15);
  TRUE(menu.set_focusSymbol(Position::LEFT, customSymbol));
  EQUAL(lcd.lastCreateCharIndex(), 14);
  TRUE(menu.set_focusSymbol(Position::CUSTOM, customSymbol));
  EQUAL(lcd.lastCreateCharIndex(), 13);
}

TEST(set_focusPosition_applies_to_every_screen_but_rejects_custom) {
  LiquidCrystal_fake lcd(16, 4);
  LiquidLine firstLine(2, 0, "first");
  LiquidLine secondLine(2, 0, "second");
  firstLine.attach_function(1, noop);
  secondLine.attach_function(1, noop);
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);

  FALSE(menu.set_focusPosition(Position::CUSTOM));
  TRUE(menu.set_focusPosition(Position::LEFT));

  menu.switch_focus();
  EQUAL(lcd.trimmedRow(0), std::string(" ") + (char)14 + "first");

  menu.next_screen();
  menu.switch_focus();
  EQUAL(lcd.trimmedRow(0), std::string(" ") + (char)14 + "second");
}

TEST(call_function_refreshes_the_display_unless_told_not_to) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine line(0, 0, "text");
  line.attach_function(1, countingCallback);
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  menu.switch_focus();
  menu.update();

  unsigned clearsBefore = lcd.clears();
  unsigned callsBefore = callCount;

  TRUE(menu.call_function(1));
  EQUAL(callCount, callsBefore + 1);
  EQUAL(lcd.clears(), clearsBefore + 1);

  TRUE(menu.call_function(1, false));
  EQUAL(callCount, callsBefore + 2);
  EQUAL(lcd.clears(), clearsBefore + 1);
}

TEST(get_currentScreen_follows_the_navigation) {
  LiquidCrystal_fake lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);

  EQUAL(menu.get_currentScreen(), &first);
  menu.next_screen();
  EQUAL(menu.get_currentScreen(), &second);
}
