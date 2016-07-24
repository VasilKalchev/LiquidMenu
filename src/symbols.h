/**
@file
Symbol definitions for the display.
*/

#pragma once

namespace symbol {

/// The default symbol for the right focus indicator
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

/// The default symbol for the left focus indicator
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

/// The default symbol for the custom focus indicator
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

} //namespace symbol
