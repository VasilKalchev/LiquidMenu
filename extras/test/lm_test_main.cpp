/**
@file
Runs every registered test case and reports the result.

Exit status is 0 only when every assertion in every case passed, so this
drops straight into `make` and into CI.
*/

#include "lm_test.h"

namespace lm_test {

TestCase *firstTest = nullptr;
unsigned checksRun = 0;
unsigned checksFailed = 0;
const char *currentTest = "";

TestCase::TestCase(const char *name, TestFunction function)
  : name(name), function(function), next(nullptr) {
  // Registration keeps declaration order, so a run reads top to bottom.
  TestCase **tail = &firstTest;
  while (*tail != nullptr) {
    tail = &(*tail)->next;
  }
  *tail = this;
}

std::string show(const std::string &value) { return "\"" + value + "\""; }
std::string show(const char *value) { return value ? "\"" + std::string(value) + "\"" : "(null)"; }
std::string show(char value) { return std::string("'") + value + "'"; }
std::string show(bool value) { return value ? "true" : "false"; }

void reportFailure(const char *file, int line, const char *expression,
                   const std::string &got, const std::string &want) {
  checksFailed++;
  std::cout << "  FAIL " << file << ":" << line << "\n"
            << "       " << expression << "\n"
            << "         got  " << got << "\n"
            << "         want " << want << "\n";
}

void checkTrue(bool condition, const char *file, int line, const char *expression) {
  checksRun++;
  if (!condition) {
    reportFailure(file, line, expression, "false", "true");
  }
}

int runAll() {
  unsigned casesRun = 0;
  unsigned casesFailed = 0;

  for (TestCase *test = firstTest; test != nullptr; test = test->next) {
    currentTest = test->name;
    unsigned failedBefore = checksFailed;
    casesRun++;

    std::cout << test->name << "\n";
    test->function();

    if (checksFailed != failedBefore) {
      casesFailed++;
    }
  }

  std::cout << "\n" << casesRun << " cases, " << checksRun << " checks, "
            << casesFailed << " cases failed, " << checksFailed
            << " checks failed\n";
  return casesFailed == 0 ? 0 : 1;
}

} //namespace lm_test

int main() {
  return lm_test::runAll();
}
