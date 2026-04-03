#include <stdint.h>
#include <time.h>
#include <string.h>
#include "buttons.h"
#include "lcd_display.h"
#include "utils.h"
#include "alarm.h"


#define TIME_TEXT_BUFF_LEN  (LCD_MAX_TEXT_LENGTH+1)
char time_text_buf[TIME_TEXT_BUFF_LEN] = "";

void loop()
{

    if(is_alarm_set())
    {        
        set_lcd_pixel(LCD_DISPLAY_WIDTH-1,0,1);
    }
    else
    {
        set_lcd_pixel(LCD_DISPLAY_WIDTH-1,0,0);
    }

    if(get_alarm_time_button_pressed()==BUTTON_ON)
    {
        if(is_alarm_set())
        {
            get_hhmm_time_string(get_alarm_time(), time_text_buf,TIME_TEXT_BUFF_LEN);
        }
        else
        {
            strcpy(time_text_buf,"--:--");
        }
        set_lcd_text(time_text_buf);
    }
    else if(get_single_digit_button_pressed()==BUTTON_ON)
    {
        set_lcd_text("  1  ");
    }
    else
    {
        get_hhmm_time_string(get_current_time(), time_text_buf,TIME_TEXT_BUFF_LEN);
        set_lcd_text(time_text_buf);
    }
}