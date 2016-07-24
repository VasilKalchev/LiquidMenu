/**
@file
Messages output macros.
*/

#pragma once

#if ESSENTIAL_MESSAGES
/// Prints to serial.
#define PRINT(x) Serial.print(x);

/// Prints to serial and starts a new line.
#define PRINTLN(x) Serial.println(x);
#else

/// Prints to serial.
#define PRINT(x)

/// Prints to serial and starts a new line.
#define PRINTLN(x)
#endif

#if DEBUGGING_MESSAGES
/// Prints to serial.
#define DEBUG(x) Serial.print(x);

/// Prints to serial and starts a new line.
#define DEBUGLN(x) Serial.println(x);

/// Prints two variables to serial.
#define DEBUG2(x, y) Serial.print(x, y);

/// Prints two variables to serial and starts a new line.
#define DEBUGLN2(x, y) Serial.println(x, y);

/// Prints to serial and records the current millisecond.
#define START(x) Serial.print(x); uint32_t start = millis();

/// Prints the time passed since START(x) was called to serial.
#define STOP() Serial.print(" OK ["); Serial.print((millis() - start) / 1000.0); Serial.println("s]");
#else

/// Prints to serial.
#define DEBUG(x)

/// Prints to serial and starts a new line.
#define DEBUGLN(x)

/// Prints two variables to serial.
#define DEBUG2(x, y)

/// Prints two variables to serial and starts a new line.
#define DEBUGLN2(x, y)

/// Prints to serial and records the current millisecond.
#define START(x)

/// Prints the time passed since START(x) was called to serial.
#define STOP()
#endif