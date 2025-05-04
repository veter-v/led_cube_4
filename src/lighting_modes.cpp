#include <Arduino.h>
#include "config.h"

// Вспомогательные функции
void switch_contacts(int dimension, const int pins[], int switch_to) {
    for (int contact = 0; contact < dimension; contact++) {
        digitalWrite(pins[contact], switch_to);
    }
}

// Режимы работы
void HorizontalLayerByLayer(int time_step) {
    for (int layer = 0; layer < DIMENSION_H; layer++) {
        digitalWrite(layerPins[layer], HIGH);
        if (layer != DIMENSION_H - 1) 
            delay(time_step);
    }

    for (int layer = 0; layer < DIMENSION_H; layer++) {
        digitalWrite(layerPins[layer], LOW);
        if (layer != DIMENSION_H - 1) 
            delay(time_step);
    }

    for (int layer = DIMENSION_H - 1; layer >= 0; layer--) {
        digitalWrite(layerPins[layer], HIGH);
        if (layer != 0) 
            delay(time_step);
    }

    for (int layer = DIMENSION_H - 1; layer >= 0; layer--) {
        digitalWrite(layerPins[layer], LOW);
        if (layer != 0) 
            delay(time_step);
    }
}

void VerticalLayerByLayer(int time_step) {

    switch_contacts(DIMENSION_H, layerPins, HIGH);
    switch_contacts(DIMENSION_S, columnPins, HIGH);
    
    for (int mask = 0; mask < DIMENSION_S; mask += DIMENSION_H) {
        for (int column = mask; column < mask + DIMENSION_H; column++) {
            digitalWrite(columnPins[column], LOW);
        }
        if (mask != DIMENSION_S - DIMENSION_H) 
            delay(time_step);
    }
    
    for (int mask = 0; mask < DIMENSION_S; mask += DIMENSION_H) {
        for (int column = mask; column < mask + DIMENSION_H; column++) {
            digitalWrite(columnPins[column], HIGH);
        }
        if (mask != DIMENSION_S - DIMENSION_H) 
            delay(time_step);
    }

    for (int mask = DIMENSION_S - 1; mask >= 0; mask -= DIMENSION_H) {
        for (int column = mask; column > mask - DIMENSION_H; column--) {
            digitalWrite(columnPins[column], LOW);
        }
        if (mask != 0) 
            delay(time_step);
    }

    for (int mask = DIMENSION_S - 1; mask >= 0; mask -= DIMENSION_H) {
        for (int column = mask; column > mask - DIMENSION_H; column--) {
            digitalWrite(columnPins[column], HIGH);
        }
        if (mask != DIMENSION_H - 1) 
            delay(time_step);
    }

    switch_contacts(DIMENSION_S, columnPins, LOW);
    switch_contacts(DIMENSION_H, layerPins, LOW);
}

void LayerByLayer(int iterations, int time_step) {
    for (int iteration = 0; iteration < iterations; iteration++) {
        HorizontalLayerByLayer(time_step);
    }
    for (int iteration = 0; iteration < iterations; iteration++) {
        VerticalLayerByLayer(time_step);
    }
}