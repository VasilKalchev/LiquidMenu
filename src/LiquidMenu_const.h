/**
@file
Constants file for LiquidMenu library.
*/

#pragma once

/*!
 * @name "LiquidCrystal" libraries enumeration.
 * These defines are used for determining the compiler messages.
 * @{
 */
/// Arduino's LiquidCrystal library (parallel)
#define LiquidCrystal_LIBRARY (1)

/// https://github.com/johnrickman/LiquidCrystal_I2C library (I2C)
#define LiquidCrystal_I2C_LIBRARY (2)

/// The test fake used by the host build (see extras/test)
/**
Selecting this includes no display header at all - `DisplayClass` is
expected to be a class that is already in scope. Only the tests use it.
*/
#define Mock_LIBRARY (255)
//!@}
