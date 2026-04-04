#include <stdint.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include "buttons.h"
#include "lcd_display.h"
#include "utils.h"
#include "alarm.h"

void loop();

int main(int argc, char **argv)
{
    /* Test Alarm Dot */
    set_alarm_time(0);
    loop();
    int pix = get_lcd_pixel(LCD_DISPLAY_WIDTH-1,0);
    printf("Alarm indicator state: %d\n", pix);
    assert(pix==0);

    /* Test to make sure the clock is displaying digits where it should */
    char * display_text = get_lcd_text();
    printf("Clock LCD text: %s\n", display_text);
    assert(display_text != 0);
    assert(isdigit(display_text[0]));
    assert(isdigit(display_text[1]));
    assert(display_text[2]==':');
    assert(isdigit(display_text[3]));
    assert(isdigit(display_text[4]));
    save_lcd_png("test_01_clock_no_alarm.png");

    /* Test the display changing to a single digit when the button is pressed */
    set_single_digit_button(1);
    loop();
    display_text = get_lcd_text();
    printf("Single digit button LCD Text: '%s'\n", display_text);
    assert(strcmp(display_text,"  1  ")==0);
    save_lcd_png("test_02_single_digit.png");

    set_single_digit_button(0);
    set_alarm_time_button(0);
    loop();
    /*
        Test the display hold, the single digit should last DISPLAY_HOLD_SECONDS
        even after the button's pressed
    */
    display_text = get_lcd_text();
    printf("LCD Text should still be the same as above: '%s'\n", display_text);
    assert(strcmp(display_text,"  1  ")==0);
    save_lcd_png("test_03_hold_active.png");

    /* Wait for the display timeout to return to the clock...*/
    sleep(DISPLAY_HOLD_SECONDS);
    loop();
    display_text = get_lcd_text();
    printf("Clock LCD text: %s\n", display_text);
    assert(display_text != 0);
    assert(isdigit(display_text[0]));
    assert(isdigit(display_text[1]));
    assert(display_text[2]==':');
    assert(isdigit(display_text[3]));
    assert(isdigit(display_text[4]));
    save_lcd_png("test_04_clock_after_hold.png");

    /* Test the alarm time display */
    set_single_digit_button(0);
    set_alarm_time_button(1);

    loop();
    display_text = get_lcd_text();
    printf("Alarm time with no alarm set LCD Text: %s\n", display_text);
    assert(strcmp(display_text,"--:--")==0);
    save_lcd_png("test_05_alarm_no_alarm_set.png");

    set_alarm_time_button(0);
    loop();
    display_text = get_lcd_text();
    /* The hold should still show the --:-- */
    assert(strcmp(display_text,"--:--")==0);

    sleep(DISPLAY_HOLD_SECONDS);
    loop();

    /* Set an alarm for 10 minutes from now and test the alarm display */
    set_alarm_time(time(NULL) + 600);
    set_alarm_time_button(1);
    loop();

    display_text = get_lcd_text();
    printf("Clock LCD text (alarm display): %s\n", display_text);
    assert(display_text != 0);
    assert(isdigit(display_text[0]));
    assert(isdigit(display_text[1]));
    assert(display_text[2]==':');
    assert(isdigit(display_text[3]));
    assert(isdigit(display_text[4]));
    save_lcd_png("test_06_alarm_time_set.png");

    sleep(DISPLAY_HOLD_SECONDS);
    loop();

    /* Now the display should go back to the clock */
    display_text = get_lcd_text();
    printf("Clock LCD text: %s\n", display_text);
    assert(display_text != 0);
    assert(isdigit(display_text[0]));
    assert(isdigit(display_text[1]));
    assert(display_text[2]==':');
    assert(isdigit(display_text[3]));
    assert(isdigit(display_text[4]));

    time_t now = get_current_time();
    set_alarm_time(now+60);
    loop();
    /* The alarm indicator should be set now that we have an alarm programmed */
    pix = get_lcd_pixel(LCD_DISPLAY_WIDTH-1,0);
    printf("Alarm indicator state: %d\n", pix);
    assert(pix==1);
    save_lcd_png("test_07_clock_alarm_dot_on.png");

    /* TODO: We could test more things like checking the actual string against the times*/

    printf("ALL TESTS PASSED!\n");
    return 0;
}
