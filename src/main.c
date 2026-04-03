#include <stdint.h>
#include <time.h>
#include <string.h>
#include "buttons.h"
#include "lcd_display.h"
#include "utils.h"
#include "alarm.h"

void loop();

int main(int argc, char **argv)
{
    while(1)
    {
        loop();
        /* Sleep for 10ms to save battery, etc. */
        sleep_ms(10);
    }
    return 0;
}

