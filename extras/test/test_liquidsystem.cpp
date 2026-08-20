/**
@file
LiquidSystem: several menus behind one interface, and the delegation of
every call to whichever menu is current.
*/

#include "lm_test.h"
#include <LiquidMenu.h>

namespace {

unsigned callCount = 0;
void countingCallback() { callCount++; }

} //namespace

TEST(add_menu_stops_at_MAX_MENUS) {
  FakeLCD lcd(16, 2);
  LiquidMenu menu(lcd);
  LiquidSystem system;

  for (uint8_t i = 0; i < MAX_MENUS; i++) {
    TRUE(system.add_menu(menu));
  }
  FALSE(system.add_menu(menu));
}

TEST(the_system_shows_the_current_menu) {
  FakeLCD lcd(16, 2);
  LiquidLine firstLine(0, 0, "menu one");
  LiquidLine secondLine(0, 0, "menu two");
  LiquidScreen firstScreen(firstLine), secondScreen(secondLine);
  LiquidMenu first(lcd, firstScreen), second(lcd, secondScreen);
  LiquidSystem system(first, second);

  system.update();
  EQUAL(lcd.trimmedRow(0), "menu one");
  EQUAL(system.get_currentMenu(), &first);
  EQUAL(system.get_currentScreen(), &firstScreen);

  TRUE(system.change_menu(second));
  EQUAL(lcd.trimmedRow(0), "menu two");
  EQUAL(system.get_currentMenu(), &second);
  EQUAL(system.get_currentScreen(), &secondScreen);
}

TEST(change_menu_rejects_a_menu_that_was_not_added) {
  FakeLCD lcd(16, 2);
  LiquidLine firstLine(0, 0, "menu one");
  LiquidLine secondLine(0, 0, "menu two");
  LiquidScreen firstScreen(firstLine), secondScreen(secondLine);
  LiquidMenu first(lcd, firstScreen), second(lcd, secondScreen);
  LiquidMenu stranger(lcd, secondScreen);
  LiquidSystem system(first, second);

  FALSE(system.change_menu(stranger));
  EQUAL(system.get_currentMenu(), &first);
}

TEST(screen_navigation_is_delegated_to_the_current_menu) {
  FakeLCD lcd(16, 2);
  LiquidLine firstLine(0, 0, "one");
  LiquidLine secondLine(0, 0, "two");
  LiquidScreen first(firstLine), second(secondLine);
  LiquidMenu menu(lcd, first, second);
  LiquidMenu other(lcd, first);
  LiquidSystem system(menu, other);

  system.update();
  EQUAL(lcd.trimmedRow(0), "one");

  system.next_screen();
  EQUAL(lcd.trimmedRow(0), "two");
  system.previous_screen();
  EQUAL(lcd.trimmedRow(0), "one");

  ++system;
  EQUAL(lcd.trimmedRow(0), "two");
  --system;
  EQUAL(lcd.trimmedRow(0), "one");

  TRUE(system.change_screen(&second));
  EQUAL(lcd.trimmedRow(0), "two");
  TRUE(system.change_screen((uint8_t)1));
  EQUAL(lcd.trimmedRow(0), "one");
}

TEST(focus_and_callbacks_are_delegated_to_the_current_menu) {
  FakeLCD lcd(16, 2);
  LiquidLine line(0, 0, "item");
  line.attach_function(1, countingCallback);
  LiquidScreen screen(line);
  LiquidMenu menu(lcd, screen);
  LiquidMenu other(lcd, screen);
  LiquidSystem system(menu, other);

  system.update();
  EQUAL(system.get_focusedLine(), 1); // parked past the last line

  system.switch_focus();
  EQUAL(system.get_focusedLine(), 0);
  EQUAL(lcd.trimmedRow(0), std::string("item") + (char)15);

  TRUE(system.is_callable(1));
  unsigned before = callCount;
  TRUE(system.call_function(1));
  EQUAL(callCount, before + 1);

  TRUE(system.set_focusedLine(0));
  TRUE(system.set_focusPosition(Position::LEFT));
  FALSE(system.set_focusPosition(Position::CUSTOM));
}
