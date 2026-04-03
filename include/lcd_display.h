#include <stdint.h>


/***
    This display assumes x=0 is the left, and y=0 is the top row
    It is also assuming that the display has a function that renders text 
    with fonts or a 7-segment display. For the upper-right, it's treating
    the display as a raw frame buffer. If the LCD display is just a frame
    buffer, fonts of some sort need to be rendered in the correct place
***/
#define LCD_DISPLAY_WIDTH 128
#define LCD_DISPLAY_HEIGHT 32
/* This is the length of the text only, you need another byte for null termination. */
#define LCD_MAX_TEXT_LENGTH 5

#define DISPLAY_HOLD_SECONDS 2

void set_lcd_pixel(uint32_t x, uint32_t y, uint32_t state);
void set_lcd_text(const char *text);

void framebuffer_clear(void);

/* For testing */
char *get_lcd_text();
int get_lcd_pixel(uint32_t x, uint32_t y);
