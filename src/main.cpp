#include <Arduino.h>

#define DIMENSION_H 4
#define DIMENSION_S 16
#define TIME_STEP 300
#define LONG_TIME 800
#define ITERATIONS 3

const int layerPins[DIMENSION_H] = {A0, A1, A2, A3}; 
const int columnPins[DIMENSION_S] = {A4, A5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

void switch_contacts(int dimension, const int pins[], int switch_to) {
    for (int contact = 0; contact < dimension; contact++) {
        digitalWrite(pins[contact], switch_to);
    }
}

void regim_HorizontalLayerByLayer(void) {
    for (int layer = 0; layer < DIMENSION_H; layer++) {
        digitalWrite(layerPins[layer], HIGH);
        if (layer != DIMENSION_H - 1) 
            delay(TIME_STEP);
    }

    for (int layer = 0; layer < DIMENSION_H; layer++) {
        digitalWrite(layerPins[layer], LOW);
        if (layer != DIMENSION_H - 1) 
            delay(TIME_STEP);
    }

    for (int layer = DIMENSION_H - 1; layer >= 0; layer--) {
        digitalWrite(layerPins[layer], HIGH);
        if (layer != 0) 
            delay(TIME_STEP);
    }

    for (int layer = DIMENSION_H - 1; layer >= 0; layer--) {
        digitalWrite(layerPins[layer], LOW);
        if (layer != 0) 
            delay(TIME_STEP);
    }
}

void regim_VerticalLayerByLayer(void) {

    switch_contacts(DIMENSION_H, layerPins, HIGH);
    switch_contacts(DIMENSION_S, columnPins, HIGH);
    
    for (int mask = 0; mask < DIMENSION_S; mask += DIMENSION_H) {
        for (int column = mask; column < mask + DIMENSION_H; column++) {
            digitalWrite(columnPins[column], LOW);
        }
        if (mask != DIMENSION_S - DIMENSION_H) 
            delay(TIME_STEP);
    }
    
    for (int mask = 0; mask < DIMENSION_S; mask += DIMENSION_H) {
        for (int column = mask; column < mask + DIMENSION_H; column++) {
            digitalWrite(columnPins[column], HIGH);
        }
        if (mask != DIMENSION_S - DIMENSION_H) 
            delay(TIME_STEP);
    }

    for (int mask = DIMENSION_S - 1; mask >= 0; mask -= DIMENSION_H) {
        for (int column = mask; column > mask - DIMENSION_H; column--) {
            digitalWrite(columnPins[column], LOW);
        }
        if (mask != 0) 
            delay(TIME_STEP);
    }

    for (int mask = DIMENSION_S - 1; mask >= 0; mask -= DIMENSION_H) {
        for (int column = mask; column > mask - DIMENSION_H; column--) {
            digitalWrite(columnPins[column], HIGH);
        }
        if (mask != DIMENSION_H - 1) 
            delay(TIME_STEP);
    }

    switch_contacts(DIMENSION_S, columnPins, LOW);
    switch_contacts(DIMENSION_H, layerPins, LOW);
}

void regim_LayerByLayer(void) {
    for (int iteration = 0; iteration < ITERATIONS; iteration++) {
        regim_HorizontalLayerByLayer();
    }
    for (int iteration = 0; iteration < ITERATIONS; iteration++) {
        regim_VerticalLayerByLayer();
    }
}

void regim_Heart(void) {
}

void regim_Cube(void) {}


void setup() {
    for (int i = 0; i < DIMENSION_H; i++) {
        pinMode(layerPins[i], OUTPUT);
        digitalWrite(layerPins[i], LOW);
    }
    for (int i = 0; i < DIMENSION_S; i++) {
        pinMode(columnPins[i], OUTPUT);
        digitalWrite(columnPins[i], LOW);
    }
}


void loop() {
    regim_LayerByLayer();
}