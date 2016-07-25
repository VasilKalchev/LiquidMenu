#pragma once

#include <Arduino.h>


#if DEBUGGING_MESSAGES
#define DEBUG(x) Serial.print(x);
#define DEBUGLN(x) Serial.println(x);
#define DEBUG2(x, y) Serial.print(x, y);
#define DEBUGLN2(x, y) Serial.println(x, y);
#else
#define DEBUG(x)
#define DEBUGLN(x)
#define DEBUG2(x, y)
#define DEBUGLN2(x, y)

#endif