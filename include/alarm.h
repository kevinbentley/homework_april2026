#ifndef ALARM_H_INCLUDED
#define ALARM_H_INCLUDED
#include <stdint.h>
#include <time.h>

void set_alarm_time(time_t t);
time_t get_alarm_time();
void clear_alarm_time();
uint32_t is_alarm_set();

#endif