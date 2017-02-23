/**
@file
Contains debug macro definitions for LiquidMenu library.
*/

#pragma once

#if LIQUIDMENU_DEBUG
  #ifdef ARDUINO_ARCH_AVR
    #include <Arduino.h>
    #define DEBUG(x) Serial.print(x);
    #define DEBUGLN(x) Serial.println(x);
    #define DEBUG2(x, y) Serial.print(x, y);
    #define DEBUGLN2(x, y) Serial.println(x, y);
  #else
    #define DEBUG(x) std::cout << x;
    #define DEBUGLN(x) std::cout << x << std::endl;
    #define DEBUG2(x, y) std::cout << x << y;
    #define DEBUGLN2(x, y) std::cout << x << y << std::endl;
  #endif
#else
  /// Normal print
  #define DEBUG(x)
  /// Print with newline
  #define DEBUGLN(x)
  /// Normal print with two parameters
  #define DEBUG2(x, y)
  /// Print two parameters with newline
  #define DEBUGLN2(x, y)
#endif
