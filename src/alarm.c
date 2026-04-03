#include "alarm.h"


/* 
    For a simple implementation, these functions could be replaced with just the alarm_time variable.
    I broke them out here so a function in the RTOS or set a hardware timer 
*/
static time_t alarm_time = 0;

void set_alarm_time(time_t t)
{
    alarm_time = t;
    /* Some hardware or RTOS call here */
}
void clear_alarm_time()
{
    alarm_time = 0;
    /* Some hardware or RTOS call here */
}

time_t get_alarm_time()
{
    return alarm_time;
}

u_int32_t is_alarm_set()
{
    if(alarm_time==0)
        return 0;
    return 1;
}

