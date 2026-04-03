#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <stdint.h>
#include <time.h>

void sleep_ms(uint32_t ms);


void get_hhmm_time_string(const time_t t, char *timebuf, int max_buf_len);
time_t get_current_time();

#endif