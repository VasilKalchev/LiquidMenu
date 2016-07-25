/**
@file
Macros for serial debugging messages.
*/

#pragma once

#include <Arduino.h>


#if DEBUGGING_MESSAGES
/// Prints to serial.
#define DEBUG(x) Serial.print(x);

/// Prints to serial and starts a new line.
#define DEBUGLN(x) Serial.println(x);

/// Prints two variables to serial.
#define DEBUG2(x, y) Serial.print(x, y);

/// Prints two variables to serial and starts a new line.
#define DEBUGLN2(x, y) Serial.println(x, y);

#else

/// Prints to serial.
#define DEBUG(x)

/// Prints to serial and starts a new line.
#define DEBUGLN(x)

/// Prints two variables to serial.
#define DEBUG2(x, y)

/// Prints two variables to serial and starts a new line.
#define DEBUGLN2(x, y)

#endif