#include <Arduino.h>

#include "config.h"
#include "lighting_modes.h"



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
    LayerByLayer(ITERATIONS, TIME_STEP);
}