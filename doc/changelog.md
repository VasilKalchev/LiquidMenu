# Changelog
All notable changes to this project will be documented in this file.


<!--
[x.y.z] - YYYY-MM-DD
### Added (for new features)
### Changed (for changes in existing functionality)
### Deprecated (for once-stable features removed in upcoming releases)
### Removed (for deprecated features removed in this release)
### Fixed (for any bug fixes)
### Security (to invite users to upgrade in case of vulnerabilities)
-->

<!-- ## [Unreleased] -->

## [1.6.0] - 2021-11-05
### Added
 - ability to call a function without updating the display
 - ability to configure focus indicator "ghosting"
 - `LiquidCrystal` as a dependency in *Arduino's library manager*
### Changed
 - dynamic text is now achieved through a `char[]` instead of from a `char*`
### Fixed
 - `LiquidSystem::is_callable` was calling `call_function` instead of `is_callable`
 - `LiquidSystem::set_focusPosition` and `LiquidSystem::set_focusSymbol` now iterate all menus
 - `change_screen` wrong parameter type

## [1.5.1] - 2019-12-12
### Added
 - accessors for the focused *line*
 - method for checking if the *line* is "callable"
### Changed
 - `change_screen` now takes a `LiquidScreen` pointer

## [1.4.1] - 2018-11-05
### Added
 - scrolling lines
 - ability to configure the amount of digits shown in the decimal part
 - support for getter functions in *lines*
 - more examples

## [1.3.0] - 2017-03-27
### Added
 - I2C interface support

## [1.2.0] - 2017-02-24
### Added
 - schematics to the examples
 - 2 new examples

## [1.1.6] - 2016-12-13
### Added
 - ability to print `const char[]` stored in `PROGMEM`

## [1.1.6-beta] - 2016-08-22
### Added
 - ability to print `const char[]` stored in `PROGMEM`

## [1.1.5] - 2016-08-16

## [1.1.4] - 2016-08-06
### Added
 - custom symbols (glyphs)
 - "soft update" method
 - ability to add variables individually
 - ability to hide *screens*

## [1.0.2] - 2016-07-27

## [1.0.1] - 2016-07-26

## [1.0.0] - 2016-07-26
