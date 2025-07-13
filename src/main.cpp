#include <Arduino.h>

#include "config.h"
#include "lighting_modes.h"
#include "button.h"

// Список режимов
void (*lighting_modes[])() = {
        AllLeds,
        HorizontalLayerByLayer,
};
/*TODO: bugfix when switching modes (init_fn/loop_fn)*/

const int mode_count = sizeof(lighting_modes) / sizeof(*lighting_modes);
int current_mode = 0;

void setup()
{
    for (int i = 0; i < DIMENSION_H; i++) {
        pinMode(layerPins[i], OUTPUT);
        digitalWrite(layerPins[i], LOW);
    }
    for (int i = 0; i < DIMENSION_S; i++) {
        pinMode(columnPins[i], OUTPUT);
        digitalWrite(columnPins[i], LOW);
    }
}

void loop()
{
    static button_modes state_btnA6 = not_pressed;
    if (check_button(state_btnA6, BUTTON_PIN)) {
        if (state_btnA6 == press) {
            current_mode = (current_mode + 1) % mode_count;
            reset_pin_state();
        } else if (state_btnA6 == hold) {
            current_mode = (current_mode - 1 + mode_count) % mode_count;
        }
    }
    lighting_modes[current_mode]();
}