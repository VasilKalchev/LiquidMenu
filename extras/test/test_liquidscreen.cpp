/**
@file
LiquidScreen: holding lines, cycling the focus, hiding, and scrolling a
screen that has more lines than the display has rows.
*/

#include "lm_test.h"
#include <LiquidMenu.h>

namespace {

void noop() {}

} //namespace

TEST(add_line_stops_at_MAX_LINES) {
  LiquidScreen screen;
  LiquidLine line(0, 0, "x");

  for (uint8_t i = 0; i < MAX_LINES; i++) {
    TRUE(screen.add_line(line));
  }
  FALSE(screen.add_line(line));
}

TEST(lines_print_in_the_order_they_were_added) {
  FakeLCD lcd(16, 4);
  LiquidLine first(0, 0, "first");
  LiquidLine second(0, 1, "second");
  LiquidLine third(0, 2, "third");
  LiquidScreen screen(first, second, third);
  LiquidMenu menu(lcd, screen);
  menu.update();

  EQUAL(lcd.trimmedRow(0), "first");
  EQUAL(lcd.trimmedRow(1), "second");
  EQUAL(lcd.trimmedRow(2), "third");
  EQUAL(lcd.trimmedRow(3), "");
}

// ~ Focus cycling ~

TEST(switch_focus_visits_every_focusable_line_then_ghosts) {
  FakeLCD lcd(16, 2);
  LiquidLine first(0, 0, "one");
  LiquidLine second(0, 1, "two");
  first.attach_function(1, noop);
  second.attach_function(1, noop);
  LiquidScreen screen(first, second);
  LiquidMenu menu(lcd, screen);

  // The focus starts parked one past the last line, so nothing is focused.
  EQUAL(menu.get_focusedLine(), 2);

  menu.switch_focus();
  EQUAL(menu.get_focusedLine(), 0);
  menu.switch_focus();
  EQUAL(menu.get_focusedLine(), 1);

  // LM_FOCUS_INDICATOR_GHOSTING is true by default, so the indicator
  // disappears for one step before wrapping round.
  menu.switch_focus();
  EQUAL(menu.get_focusedLine(), 2);
  menu.switch_focus();
  EQUAL(menu.get_focusedLine(), 0);
}

TEST(switch_focus_backwards_walks_the_other_way) {
  FakeLCD lcd(16, 2);
  LiquidLine first(0, 0, "one");
  LiquidLine second(0, 1, "two");
  first.attach_function(1, noop);
  second.attach_function(1, noop);
  LiquidScreen screen(first, second);
  LiquidMenu menu(lcd, screen);

  menu.switch_focus(false);
  EQUAL(menu.get_focusedLine(), 1);
  menu.switch_focus(false);
  EQUAL(menu.get_focusedLine(), 0);
  menu.switch_focus(false);
  EQUAL(menu.get_focusedLine(), 2); // the ghost step
}

TEST(switch_focus_skips_lines_without_a_function) {
  FakeLCD lcd(16, 4);
  LiquidLine title(0, 0, "title");
  LiquidLine first(0, 1, "one");
  LiquidLine second(0, 2, "two");
  first.attach_function(1, noop);
  second.attach_function(1, noop);
  LiquidScreen screen(title, first, second);
  LiquidMenu menu(lcd, screen);

  menu.switch_focus();
  EQUAL(menu.get_focusedLine(), 1); // the title is skipped
  menu.switch_focus();
  EQUAL(menu.get_focusedLine(), 2);
}

TEST(set_focusedLine_accepts_only_focusable_lines) {
  FakeLCD lcd(16, 2);
  LiquidLine title(0, 0, "title");
  LiquidLine item(0, 1, "item");
  item.attach_function(1, noop);
  LiquidScreen screen(title, item);
  LiquidMenu menu(lcd, screen);

  FALSE(menu.set_focusedLine(0));  // no function attached
  TRUE(menu.set_focusedLine(1));
  EQUAL(menu.get_focusedLine(), 1);
  FALSE(menu.set_focusedLine(9));  // past the end
  EQUAL(menu.get_focusedLine(), 1);
}

// ~ Hiding ~

TEST(a_hidden_screen_is_skipped_when_changing_screens) {
  FakeLCD lcd(16, 2);
  LiquidLine firstLine(0, 0, "first");
  LiquidLine secondLine(0, 0, "second");
  LiquidLine thirdLine(0, 0, "third");
  LiquidScreen first(firstLine), second(secondLine), third(thirdLine);
  LiquidMenu menu(lcd, first, second, third);

  second.hide(true);
  menu.next_screen();
  EQUAL(lcd.trimmedRow(0), "third");

  second.hide(false);
  menu.previous_screen();
  EQUAL(lcd.trimmedRow(0), "second");
}

// ~ Scrolling ~

TEST(set_displayLineCount_scrolls_the_focused_line_into_view) {
  FakeLCD lcd(16, 2);
  LiquidLine one(0, 0, "one");
  LiquidLine two(0, 1, "two");
  LiquidLine three(0, 2, "three");
  LiquidLine four(0, 3, "four");
  one.attach_function(1, noop);
  two.attach_function(1, noop);
  three.attach_function(1, noop);
  four.attach_function(1, noop);

  LiquidScreen screen(one, two, three, four);
  screen.set_displayLineCount(2); // a 2 row display, 4 lines of content
  LiquidMenu menu(lcd, screen);

  menu.switch_focus(); // line 0
  menu.update();
  EQUAL(lcd.trimmedRow(0), std::string("one") + (char)15);
  EQUAL(lcd.trimmedRow(1), "two");

  menu.switch_focus(); // line 1, still on screen
  EQUAL(lcd.trimmedRow(0), "one");
  EQUAL(lcd.trimmedRow(1), std::string("two") + (char)15);

  menu.switch_focus(); // line 2 scrolls the window down by one
  EQUAL(lcd.trimmedRow(0), "two");
  EQUAL(lcd.trimmedRow(1), std::string("three") + (char)15);

  menu.switch_focus(); // line 3
  EQUAL(lcd.trimmedRow(0), "three");
  EQUAL(lcd.trimmedRow(1), std::string("four") + (char)15);
}
