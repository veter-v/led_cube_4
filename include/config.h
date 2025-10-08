#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define BUTTON_PIN A6
#define DIMENSION_H 4
#define DIMENSION_S 16
#define DIMENSION_S_BORDER 2 * (DIMENSION_H + DIMENSION_H - 2)

#define TIME_STEP 300

#define TIME_HEART_KNOCK 80
#define TIME_HEART_PHASE 1800
#define MAX_KNOCKS 2

extern const uint8_t layerPins[DIMENSION_H];
extern const uint8_t columnPins[DIMENSION_S];
extern const uint8_t borderColumnPins[DIMENSION_S_BORDER];

#endif