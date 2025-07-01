#ifndef __BUTTON_H__
#define __BUTTON_H__

#define DEBOUNCE_TIME 50
#define LONG_PRESS_TIME 600
#define ANALOG_TRESHOLD 500

enum button_modes { not_pressed, press, hold };

bool check_button(button_modes& state, uint8_t analog_pin);

#endif