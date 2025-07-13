#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define BUTTON_PIN A6
#define DIMENSION_H 4
#define DIMENSION_S 16

#define TIME_STEP 300
#define ITERATIONS 3

extern const uint8_t layerPins[DIMENSION_H];
extern const uint8_t columnPins[DIMENSION_S];

#endif