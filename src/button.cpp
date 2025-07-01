#include <Arduino.h>
#include "button.h"

bool read_analog_button(uint8_t analog_pin)
{
    return analogRead(analog_pin) < ANALOG_TRESHOLD;
}

bool check_button(button_modes& state, uint8_t analog_pin)
{
    static uint32_t btn_start_timer;
    uint32_t current_time = millis();

    static bool is_hold_triggered;
    static bool last_state;
    bool current_state = read_analog_button(analog_pin);

    if (last_state != current_state) {
        last_state = current_state;
        if (current_state) {
            // Начало нажатия
            btn_start_timer = current_time;
            is_hold_triggered = false;
        } else if (!is_hold_triggered && (current_time - btn_start_timer > DEBOUNCE_TIME) &&
                   (current_time - btn_start_timer < LONG_PRESS_TIME)) {
            // Конец короткого нажатия
            state = press;
            return true;
        }
    }

    if (!is_hold_triggered && current_state && (current_time - btn_start_timer >= LONG_PRESS_TIME)) {
        // Попадение в длинное нажатие
        state = hold;
        is_hold_triggered = true;
        return true;
    }

    state = not_pressed;
    return false;
}
