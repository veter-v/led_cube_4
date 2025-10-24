#include <Arduino.h>
#include "button.h"

bool Button::check_button()
{
    uint32_t current_time = millis();

    bool current_state = read_analog_button();

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

    if (!is_hold_triggered && current_state &&
        (current_time - btn_start_timer >= LONG_PRESS_TIME)) {
        // Попадение в длинное нажатие
        state = hold;
        is_hold_triggered = true;
        return true;
    }

    state = not_pressed;
    return false;
}
