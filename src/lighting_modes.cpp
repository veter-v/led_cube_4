#include <Arduino.h>
#include "config.h"

// Вспомогательные функции
void reset_pin_state()
{
    for (int i = 0; i < DIMENSION_H; i++) {
        digitalWrite(layerPins[i], LOW);
    }
    for (int i = 0; i < DIMENSION_S; i++) {
        digitalWrite(columnPins[i], LOW);
    }
}

// Режимы работы
void AllLeds()
{
    static uint8_t layer = 0;
    static uint32_t timer = millis();
    static bool switch_led_to;

    if (layer == DIMENSION_H) {
        if (millis() - timer >= TIME_STEP) {
            switch_led_to = !switch_led_to;
            layer = 0;
        }
    } else {
        digitalWrite(layerPins[layer++], switch_led_to);
        if (layer == DIMENSION_H) {
            timer = millis();
        }
    }
}

void HorizontalLayerByLayer()
{
    static int phase = 0;
    static int layer = 0;
    static unsigned long last_step_time = 0;

    if (millis() - last_step_time < TIME_STEP)
        return;

    last_step_time = millis();

    switch (phase) {
    case 0: // Включаем по порядку
        digitalWrite(layerPins[layer], HIGH);
        layer++;
        if (layer >= DIMENSION_H) {
            layer = 0;
            phase = 1;
        }
        break;

    case 1: // Выключаем по порядку
        digitalWrite(layerPins[layer], LOW);
        layer++;
        if (layer >= DIMENSION_H) {
            layer = DIMENSION_H - 1;
            phase = 2;
        }
        break;

    case 2: // Включаем в обратном порядке
        digitalWrite(layerPins[layer], HIGH);
        layer--;
        if (layer < 0) {
            layer = DIMENSION_H - 1;
            phase = 3;
        }
        break;

    case 3: // Выключаем в обратном порядке
        digitalWrite(layerPins[layer], LOW);
        layer--;
        if (layer < 0) {
            layer = 0;
            phase = 0;
        }
        break;
    }
}