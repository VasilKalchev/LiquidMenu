/**
@file
Contains glyph definitions for LiquidMenu library.
*/

#pragma once

namespace glyph {
/// The focus glyph shown on the right.
uint8_t rightFocus[8] = {
	0b00000,
	0b00100,
	0b01000,
	0b11111,
	0b01000,
	0b00100,
	0b00000,
	0b00000
};

/// The focus glyph shown on the left.
uint8_t leftFocus[8] = {
	0b00000,
	0b00100,
	0b00010,
	0b11111,
	0b00010,
	0b00100,
	0b00000,
	0b00000
};

/// The focus glyph shown when custom focus is set.
uint8_t customFocus[8] = {
	0b00000,
	0b00000,
	0b01110,
	0b01110,
	0b01110,
	0b00000,
	0b00000,
	0b00000
};
} //namespace glyph
