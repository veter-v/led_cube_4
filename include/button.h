#pragma once

#include <Arduino.h>

#define DEBOUNCE_TIME 50
#define LONG_PRESS_TIME 600
#define ANALOG_TRESHOLD 500

class Button {
  public:
    enum button_modes { not_pressed, press, hold };

  private:
    uint8_t pin;
    button_modes state = not_pressed;

    bool read_analog_button() const { return analogRead(pin) < ANALOG_TRESHOLD; }

  public:
    Button(uint8_t pin) : pin(pin), state(not_pressed) {}

    button_modes get_state() const { return state; }
    bool check_button();
};
