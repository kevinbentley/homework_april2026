#include <stdint.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#include "buttons.h"
#include "lcd_display.h"
#include "utils.h"
#include "alarm.h"

void loop();

int main(int argc, char **argv)
{
    set_alarm_time(0);
    loop();
    int pix = get_lcd_pixel(LCD_DISPLAY_WIDTH-1,0);
    printf("Alarm indicator state: %d\n", pix);
    assert(pix==0);
    char * display_text = get_lcd_text();
    printf("Clock LCD text: %s\n", display_text);
    assert(display_text != 0);
    assert(isdigit(display_text[0]));
    assert(isdigit(display_text[1]));
    assert(display_text[2]==':');
    assert(isdigit(display_text[3]));
    assert(isdigit(display_text[4]));
    
    set_single_digit_button(1);
    loop();
    display_text = get_lcd_text();
    printf("Single digit button LCD Text: '%s'\n", display_text);
    assert(strcmp(display_text,"  1  ")==0);
    
    set_single_digit_button(0);    
    set_alarm_time_button(1);
    loop();
    display_text = get_lcd_text();
    printf("Alarm time with no alarm set LCD Text: %s\n", display_text);
    assert(strcmp(display_text,"--:--")==0);

    time_t now = get_current_time();
    set_alarm_time(now+60);
    loop();
    pix = get_lcd_pixel(LCD_DISPLAY_WIDTH-1,0);
    printf("Alarm indicator state: %d\n", pix);
    assert(pix==1);
    
    printf("Alarm LCD text: %s\n", display_text);
    assert(display_text != 0);
    assert(isdigit(display_text[0]));
    assert(isdigit(display_text[1]));
    assert(display_text[2]==':');
    assert(isdigit(display_text[3]));
    assert(isdigit(display_text[4]));
    
    /* We could test more things like checking the actual string against the times*/

    printf("ALL TESTS PASSED!\n");
    return 0;
}
