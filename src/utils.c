#include <string.h>
#include <stdio.h>
#include "utils.h"

void sleep_ms(uint8_t ms)
{
    /* This would map to some hardware tick rate calculation or RTOS function in practice. */
    struct timespec ts = { .tv_sec = 0, .tv_nsec = ms * 1000000 };
    nanosleep(&ts, NULL);
}

void get_hhmm_time_string(const time_t t, char *timebuf, int max_buf_len)
{
    struct tm tm;
    localtime_r(&t, &tm);
    snprintf(timebuf, max_buf_len, "%02d:%02d", tm.tm_hour, tm.tm_min);
}

time_t get_current_time()
{
    return time(NULL);
}