#include "buttons.h"

/* These variables might be set in an ISR or the gpio might be read directly in the functions instead*/
/* Button one is for displaying the alarm time */
uint32_t button_alarm_time_state = 0;
/* Button two is to show a single digit */
uint32_t button_single_digit_state = 0;

uint32_t get_alarm_time_button_pressed()
{
    /* gpio read could be here as an alternative */
    return button_alarm_time_state;
}

uint32_t get_single_digit_button_pressed()
{
    return button_single_digit_state;
}

/* For tests */
void set_alarm_time_button(uint32_t state)
{
    button_alarm_time_state = state;
}
void set_single_digit_button(uint32_t state)
{
    button_single_digit_state = state;
}
