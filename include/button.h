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
    button_modes state {not_pressed};

    uint32_t btn_start_timer {0};
    bool is_hold_triggered {false};
    bool last_state {false};

    bool read_analog_button() const { return analogRead(pin) < ANALOG_TRESHOLD; }

  public:
    Button(uint8_t pin)
        : pin(pin), state(not_pressed), btn_start_timer(0), is_hold_triggered(false),
          last_state(false)
    {
    }

    button_modes get_state() const { return state; }
    bool check_button();
};
