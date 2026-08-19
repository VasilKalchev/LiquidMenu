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
//!@}

/*!
 * @name Focus indicator custom character slots.
 * The character generator RAM slots used for the built-in focus glyphs.
 * @{
 */
#define LM_FOCUS_GLYPH_CUSTOM (13)
#define LM_FOCUS_GLYPH_LEFT (14)
#define LM_FOCUS_GLYPH_RIGHT (15)
//!@}
