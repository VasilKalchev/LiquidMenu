/**
@file
Configuration file for LiquidMenu library.

Contains global constants the configure the size of some of the arrays
used in the library, also configures the debugging messages.
*/

#pragma once

/*!
 * @name "LiquidCrystal" libraries enumeration.
 * These defines are used for determining the compiler messages.
 * @{
 */
#define LiquidCrystal_LIBRARY (1)
#define LiquidCrystal_I2C_LIBRARY (2)
//!@}


// Select a "LiquidCrystal" library:
// ---------------------------------

/*!
 * @name Select a "LiquidCrystal" library
 * @{
 */

/*!
 * @name Arduino's parallel "LiquidCrystal" library
 * @{
 */
#define LIQUIDMENU_LIBRARY LiquidCrystal_LIBRARY
#include <LiquidCrystal.h>
#define DisplayClass LiquidCrystal
//!@}

/*!
 * @name I2C library
 * @see https://github.com/johnrickman/LiquidCrystal_I2C
 * @{
 */
// #define LIQUIDMENU_LIBRARY LiquidCrystal_I2C_LIBRARY
// #include <LiquidCrystal_I2C.h>
// #define DisplayClass LiquidCrystal_I2C
//!@}

/*!
 * @name Some other library
 * @{
 */
// #include <LIBRARY_HEADER.h>
// #define DisplayClass LIBRARY_CONSTRUCTOR
//!@}
//!@}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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
