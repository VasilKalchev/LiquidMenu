#pragma once

/*
 * A minimal debounced push-button helper.
 *
 * This class is NOT part of the LiquidMenu library - it is bundled with
 * the examples so that they compile without any additional dependency.
 * Wire the button between the pin and ground and leave "pullup" enabled.
 */
class Button {
public:
  explicit Button(uint8_t pin, bool pullup = true, uint16_t debounceDelay = 25)
    : _pin(pin), _debounceDelay(debounceDelay),
      _state(HIGH), _lastReading(HIGH), _lastChangeMs(0) {
    pinMode(_pin, pullup ? INPUT_PULLUP : INPUT);
  }

  /*
   * Returns true once per press, on the moment the button goes down.
   * Call it on every loop() iteration - the debouncing relies on that.
   */
  bool pressed() {
    bool reading = digitalRead(_pin);

    // The reading changed - (re)start the debounce timer.
    if (reading != _lastReading) {
      _lastReading = reading;
      _lastChangeMs = millis();
      return false;
    }

    // The reading has been stable long enough to be accepted.
    if ((millis() - _lastChangeMs) > _debounceDelay && reading != _state) {
      _state = reading;
      // With a pull-up resistor a pressed button reads LOW.
      return (_state == LOW);
    }

    return false;
  }

private:
  const uint8_t _pin;
  const uint16_t _debounceDelay;
  bool _state;
  bool _lastReading;
  uint32_t _lastChangeMs;
};
