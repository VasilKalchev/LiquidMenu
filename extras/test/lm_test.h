/**
@file
A very small test framework - just enough to register test cases and
compare values, with no external dependency to install or vendor.

A test file declares cases with `TEST(name) { ... }`; they register
themselves before `main()` runs and `lm_test_main.cpp` executes all of
them. Assertions record a failure and keep going, so one broken case
reports every one of its problems in a single run.
*/

#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace lm_test {

typedef void (*TestFunction)();

/// One registered test case, linked into a global list.
struct TestCase {
  TestCase(const char *name, TestFunction function);
  const char *name;
  TestFunction function;
  TestCase *next;
};

/// Head of the registration list and the counters, defined in lm_test_main.cpp.
extern TestCase *firstTest;
extern unsigned checksRun;
extern unsigned checksFailed;
extern const char *currentTest;

int runAll();

/// Renders a value for a failure message; strings get quoted and made visible.
template <typename T>
inline std::string show(const T &value) {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

std::string show(const std::string &value);
std::string show(const char *value);
std::string show(char value);
std::string show(bool value);

void reportFailure(const char *file, int line, const char *expression,
                   const std::string &got, const std::string &want);

template <typename A, typename B>
inline void check(const A &got, const B &want, const char *file, int line,
                  const char *expression) {
  checksRun++;
  if (!(got == want)) {
    reportFailure(file, line, expression, show(got), show(want));
  }
}

template <typename A, typename B>
inline void checkNot(const A &got, const B &unwanted, const char *file, int line,
                     const char *expression) {
  checksRun++;
  if (got == unwanted) {
    reportFailure(file, line, expression, show(got), "anything else");
  }
}

void checkTrue(bool condition, const char *file, int line, const char *expression);

} //namespace lm_test

/// Declares and registers a test case.
#define TEST(name)                                                    \
  static void name();                                                 \
  static lm_test::TestCase name##_registration(#name, name);          \
  static void name()

/// Fails the current test if `got != want`, then continues.
#define EQUAL(got, want) \
  lm_test::check((got), (want), __FILE__, __LINE__, #got)

/// Fails the current test if `got == unwanted`, then continues.
#define NOT_EQUAL(got, unwanted) \
  lm_test::checkNot((got), (unwanted), __FILE__, __LINE__, #got)

/// Fails the current test if the condition is false.
#define TRUE(condition) \
  lm_test::checkTrue((condition), __FILE__, __LINE__, #condition)

#define FALSE(condition) \
  lm_test::checkTrue(!(condition), __FILE__, __LINE__, "!(" #condition ")")

/**
Pins down behaviour that is known to be *wrong*, so that fixing it fails
loudly here instead of silently changing what users see. Reads as
`KNOWN_BUG(1, "char* prints the pointer, not the string");` followed by the
assertions describing today's output.

When the bug is fixed, the assertions below the marker are the ones to
rewrite - and the marker is what leads you to them.
*/
#define KNOWN_BUG(number, description) \
  ((void)sizeof(description)) // documentation only, see extras/test/README.md
