#pragma once

/*
 * A minimal polled quadrature rotary encoder helper (KY-040 style).
 *
 * This class is NOT part of the LiquidMenu library - it is bundled with
 * the example so that it compiles without any additional dependency.
 * It counts one step per detent by looking at the DT line on the falling
 * edge of the CLK line. For a production project prefer an interrupt
 * driven encoder library.
 */
class Encoder {
public:
  Encoder(uint8_t pinCLK, uint8_t pinDT)
    : _pinCLK(pinCLK), _pinDT(pinDT), _lastCLK(HIGH) {
    pinMode(_pinCLK, INPUT_PULLUP);
    pinMode(_pinDT, INPUT_PULLUP);
  }

  /*
   * Returns -1 (counter-clockwise), +1 (clockwise) or 0 (no movement).
   * Call it on every loop() iteration so that no step is missed.
   */
  int8_t read() {
    bool clk = digitalRead(_pinCLK);
    int8_t direction = 0;

    if (_lastCLK == HIGH && clk == LOW) {
      // A detent was reached - DT tells which way the knob turned.
      direction = (digitalRead(_pinDT) == HIGH) ? 1 : -1;
    }
    _lastCLK = clk;

    return direction;
  }

private:
  const uint8_t _pinCLK;
  const uint8_t _pinDT;
  bool _lastCLK;
};
