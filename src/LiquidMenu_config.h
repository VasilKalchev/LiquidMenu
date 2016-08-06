/**
@file
Configuration file for LiquidMenu library.

Contains global constants the configure the size of some of the arrays
used in the library, also configures the messages output.
*/

#pragma once

/// Configures the numer of available variables per line.
const uint8_t MAX_VARIABLES = 5; ///< @note Default: 5

/// Configures the number of available functions per line.
const uint8_t MAX_FUNCTIONS = 8; ///< @note Default: 8

/// Configures the number of available lines per screen.
const uint8_t MAX_LINES = 12; ///< @note Default: 12

/// Configures the number of available screens per menu.
const uint8_t MAX_SCREENS = 16; ///< @note Default: 16

/// Configures the number of available manus per menus system.
const uint8_t MAX_MENUS = 12; ///< @note Defaul: 12

// Turns the debugging messages on or off.
#define DEBUGGING_MESSAGES false ///< @note Default: false
