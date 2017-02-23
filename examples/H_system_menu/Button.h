#pragma once

class Button {
public:
	Button (uint8_t pin, bool pullup = false, uint16_t debounceDelay = 50)
		: _pin(pin), _state(LOW), _lastState(LOW),
		  _lastMillis(0), _debounceDelay(debounceDelay),
		  _lastDebounceTime(0) {
		if (pullup == true) {
			pinMode(_pin, INPUT_PULLUP);
		} else {
			pinMode(_pin, INPUT);
		}
	}

	// Debounces the button and returns the state if it was just changed.
	bool check(bool triggerState = LOW) {
		bool reading = digitalRead(_pin);
		// Checks if the buttons has changed state
		if (reading != _lastState) {
			_lastDebounceTime = millis();
		}
		// Checks if the buttons hasn't changed state for '_debounceDelay' milliseconds.
		if ((millis() - _lastDebounceTime) > _debounceDelay) {
			// Checks if the buttons has changed state
			if (reading != _state) {
				_state = reading;
				return _state;
			}
		}
		_lastState = reading;
		// If this code is reached, it returns the normal state of the button.
		if (triggerState == HIGH) {
			return LOW;
		} else {
			return HIGH;
		}
	}

private:
	const uint8_t _pin;
	bool _state;
	bool _lastState;
	uint32_t _lastMillis;
	uint16_t _debounceDelay;
	uint32_t _lastDebounceTime;
};
