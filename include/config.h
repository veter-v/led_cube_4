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

inline constexpr uint8_t layerPins[DIMENSION_H] = {A0, A1, A2, A3};
inline constexpr uint8_t columnPins[DIMENSION_S] = {
        A4, A5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
inline constexpr uint8_t borderColumnPins[DIMENSION_S_BORDER] = {
        A4, A5, 0, 1, 2, 5, 6, 9, 10, 11, 12, 13};

#endif