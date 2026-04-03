#include "lcd_display.h"
#include <string.h>

static char display_text[LCD_MAX_TEXT_LENGTH+1] = "";
/* The +7 makes sure we round up for room if we have an odd size */
static uint8_t framebuffer[(LCD_DISPLAY_WIDTH * LCD_DISPLAY_HEIGHT + 7) / 8];

/* 
    Even though we have a full framebuffer, this code assumes there's a 'write text' 
    part of the driver 
*/
void set_lcd_text(const char *text)
{
    strncpy(display_text,text,LCD_MAX_TEXT_LENGTH);
    /* Some call to a driver that writes to text goes here. */
}

/* For testing */
char *get_lcd_text()
{
    return display_text;
}

/* I used AI to write the functions below (mostly)*/
void framebuffer_clear(void)
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void set_lcd_pixel(uint32_t x, uint32_t y, uint32_t on)
{
    if (x < 0 || x >= LCD_DISPLAY_WIDTH || y < 0 || y >= LCD_DISPLAY_HEIGHT)
        return;

    uint32_t bit_index = y * LCD_DISPLAY_WIDTH + x;
    uint32_t byte_offset = bit_index / 8;
    uint32_t bit_offset = bit_index % 8;

    if (on)
        framebuffer[byte_offset] |= (1 << bit_offset);
    else
        framebuffer[byte_offset] &= ~(1 << bit_offset);
}

int get_lcd_pixel(uint32_t x, uint32_t y)
{
    if (x < 0 || x >= LCD_DISPLAY_WIDTH || y < 0 || y >= LCD_DISPLAY_HEIGHT)
        return 0;

    uint32_t bit_index = y * LCD_DISPLAY_WIDTH + x;
    uint32_t byte_offset = bit_index / 8;
    uint32_t bit_offset = bit_index % 8;

    return (framebuffer[byte_offset] >> bit_offset) & 1;
}