#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#include <stdint.h>

#define BUTTON_ON 1
#define BUTTON_OFF 0

uint32_t get_alarm_time_button_pressed();
uint32_t get_single_digit_button_pressed();

/* These are just for testing */
void set_alarm_time_button(uint32_t state);
void set_single_digit_button(uint32_t state);

#endif