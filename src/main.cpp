#include <Arduino.h>

#include "config.h"
#include "lighting_modes.h"
#include "button.h"

// Временные ф-ии
void mode1()
{
    Serial.println("mode1");
}
void mode2()
{
    Serial.println("mode2");
}
void mode3()
{
    Serial.println("mode3");
}

void (*lighting_modes[])() = {
        mode1,
        mode2,
        mode3,
};

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

    Serial.begin(9600);
}

void loop()
{
    static button_modes state_btnA6 = not_pressed;
    if (check_button(state_btnA6, BUTTON_PIN)) {
        if (state_btnA6 == press) {
            current_mode = (current_mode + 1) % mode_count;
        } else if (state_btnA6 == hold) {
            current_mode = (current_mode - 1 + mode_count) % mode_count;
        }
    }
    lighting_modes[current_mode]();
}