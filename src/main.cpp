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
    Button btn_switch_modes(BUTTON_PIN);
    if (btn_switch_modes.check_button()) {
        if (btn_switch_modes.get_state() == Button::press) {
            current_mode = (current_mode + 1) % mode_count;
            reset_pin_state();
        } else if (btn_switch_modes.get_state() == Button::hold) {
            current_mode = (current_mode - 1 + mode_count) % mode_count;
        }
    }
    lighting_modes[current_mode]();
}