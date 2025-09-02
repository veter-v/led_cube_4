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
    counter = 0;
    timer = millis();
    phase = 0;
    switch_led_to = !start_reset_to;
}

// Режимы работы
void AllLeds::mode()
{
    if (counter == DIMENSION_H) {
        if (millis() - timer >= TIME_STEP) {
            switch_led_to = !switch_led_to;
            counter = 0;
        }
    } else {
        digitalWrite(layerPins[counter++], switch_led_to);
        if (counter == DIMENSION_H) {
            timer = millis();
        }
    }
}

void HorizontalLayerByLayer::mode()
{
    if (millis() - timer < TIME_STEP)
        return;

    timer = millis();
    switch (phase) {
    case 0: // Включаем по порядку
        digitalWrite(layerPins[counter], HIGH);
        counter++;
        if (counter >= DIMENSION_H) {
            counter = 0;
            phase = 1;
        }
        break;

    case 1: // Выключаем по порядку
        digitalWrite(layerPins[counter], LOW);
        counter++;
        if (counter >= DIMENSION_H) {
            counter = DIMENSION_H - 1;
            phase = 2;
        }
        break;

    case 2: // Включаем в обратном порядке
        if (counter > 0) {
            digitalWrite(layerPins[counter], HIGH);
            counter--;
        } else if (counter == 0) {
            digitalWrite(layerPins[counter], HIGH);
            counter = DIMENSION_H - 1;
            phase = 3;
        }
        break;

    case 3: // Выключаем в обратном порядке
        if (counter > 0) {
            digitalWrite(layerPins[counter], LOW);
            counter--;
        } else if (counter == 0) {
            digitalWrite(layerPins[counter], LOW);
            phase = 0;
        }
        break;
    }
}