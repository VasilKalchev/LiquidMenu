/**
@file
Configuration file for LiquidMenu library.

Contains global constants the configure the size of some of the arrays
used in the library, also configures the debugging messages.
*/

#pragma once

/// Switches the communication method to I2C
#define I2C false ///< @note Default: false

/// Configures the number of available variables per line.
const uint8_t MAX_VARIABLES = 5; ///< @note Default: 5

/// Configures the number of available functions per line.
const uint8_t MAX_FUNCTIONS = 8; ///< @note Default: 8

/// Configures the number of available lines per screen.
const uint8_t MAX_LINES = 12; ///< @note Default: 12

/// Configures the number of available screens per menu.
const uint8_t MAX_SCREENS = 14; ///< @note Default: 14

/// Configures the number of available menus per menus system.
const uint8_t MAX_MENUS = 8; ///< @note Default: 8

// Turns the debugging messages on or off.
#define LIQUIDMENU_DEBUG false ///< @note Default: false
