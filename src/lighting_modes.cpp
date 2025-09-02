#include <Arduino.h>
#include "config.h"
#include "lighting_modes.h"

// Mode
void Mode::reset_pin_state() const
{
    for (int i = 0; i < DIMENSION_H; i++) {
        digitalWrite(layerPins[i], start_reset_to);
    }
    for (int i = 0; i < DIMENSION_S; i++) {
        digitalWrite(columnPins[i], start_reset_to);
    }
}

void Mode::set_to_start()
{
    pins_counter = 0;
    timer = millis();
    phase = 0;
}

// Режимы работы
void AllLeds::mode()
{
    if (pins_counter == DIMENSION_H) {
        if (millis() - timer >= TIME_STEP) {
            switch_led_to = !switch_led_to;
            pins_counter = 0;
        }
    } else {
        digitalWrite(layerPins[pins_counter++], switch_led_to);
        if (pins_counter == DIMENSION_H) {
            timer = millis();
        }
    }
}

void AllLeds::set_to_start()
{
    Mode::set_to_start();
    switch_led_to = !switch_led_to;
}

void HorizontalLayerByLayer::mode()
{
    if (millis() - timer < TIME_STEP)
        return;

    timer = millis();
    switch (phase) {
    case 0: // Включаем по порядку
        digitalWrite(layerPins[pins_counter], HIGH);
        pins_counter++;
        if (pins_counter == DIMENSION_H) {
            pins_counter = 0;
            phase = 1;
        }
        break;

    case 1: // Выключаем по порядку
        digitalWrite(layerPins[pins_counter], LOW);
        pins_counter++;
        if (pins_counter == DIMENSION_H - 1) {
            pins_counter = DIMENSION_H - 1;
            phase = 2;
        }
        break;

    case 2: // Включаем в обратном порядке
        if (pins_counter > 0) {
            digitalWrite(layerPins[pins_counter], HIGH);
            pins_counter--;
        } else if (pins_counter == 0) {
            digitalWrite(layerPins[pins_counter], HIGH);
            pins_counter = DIMENSION_H - 1;
            phase = 3;
        }
        break;

    case 3: // Выключаем в обратном порядке
        if (pins_counter > 0) {
            digitalWrite(layerPins[pins_counter], LOW);
            pins_counter--;
        } else if (pins_counter == 0) {
            phase = 0;
        }
        break;
    }
}