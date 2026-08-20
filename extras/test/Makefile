# Host build of LiquidMenu's test suite. No board and no Arduino
# installation involved - the library is compiled for this machine against
# the fake display in FakeLCD.h.
#
#   make            build and run
#   make asan       the same, with the address/UB sanitizers
#   make unsafe     also run the cases that demonstrate bug 1, which read
#                   and call through addresses that are not ours and so
#                   crash rather than fail - see test_variables.cpp
#   make clean
#
# See README.md.

SRC_DIR   := ../../src
BUILD_DIR := build
BINARY    := $(BUILD_DIR)/lm_test

CXX      ?= g++
CXXSTD   ?= c++11
WARNINGS := -Wall -Wextra

# The library's only tie to a display is the DisplayClass macro; pointing it
# at the fake is the whole of the substitution. Mock_LIBRARY suppresses the
# #include of a real "LiquidCrystal" header.
DEFINES  := -DLIQUIDMENU_LIBRARY=Mock_LIBRARY -DDisplayClass=FakeLCD
INCLUDES := -I. -I$(SRC_DIR)

# The library headers name DisplayClass and the AVR flash macros before any
# test file gets a chance to include something, so both are forced in first.
FORCED   := -include arduino_compat.h -include FakeLCD.h

CXXFLAGS := -std=$(CXXSTD) $(WARNINGS) -g -O0 $(DEFINES) $(INCLUDES) $(FORCED)

LIBRARY_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_SOURCES    := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(LIBRARY_SOURCES)) $(TEST_SOURCES))

.PHONY: all test asan unsafe clean

all: test

test: $(BINARY)
	@./$(BINARY)

# Bugs in this library are mostly memory bugs, and the host is the only place
# they can be detected automatically.
# Each variant builds into its own directory, so switching between them
# never links objects compiled with different flags.
asan:
	@$(MAKE) --no-print-directory test BUILD_DIR=build-asan \
		CXXFLAGS="-std=$(CXXSTD) $(WARNINGS) -g -O1 $(DEFINES) $(INCLUDES) $(FORCED) -fsanitize=address,undefined -fno-omit-frame-pointer" \
		LDFLAGS="-fsanitize=address,undefined"

unsafe:
	@$(MAKE) --no-print-directory test BUILD_DIR=build-unsafe \
		CXXFLAGS="$(CXXFLAGS) -DLM_TEST_UNSAFE_KNOWN_BUGS=1"

$(BINARY): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	@rm -rf build build-asan build-unsafe

-include $(OBJECTS:.o=.d)
