#include "lcd_display.h"
#include "font5x7.h"
#include <string.h>

static char display_text[LCD_MAX_TEXT_LENGTH+1] = "";
/* The +7 makes sure we round up for room if we have an odd size */
static uint8_t framebuffer[(LCD_DISPLAY_WIDTH * LCD_DISPLAY_HEIGHT + 7) / 8];

/* Vertical offset to center 7px tall text in 32px display */
#define TEXT_START_Y  ((LCD_DISPLAY_HEIGHT - FONT_CHAR_HEIGHT) / 2)

/*
    Renders text string into the framebuffer using the 5x7 bitmap font,
    centered horizontally and vertically. Also stores the string for
    get_lcd_text() compatibility.
*/
void set_lcd_text(const char *text)
{
    strncpy(display_text, text, LCD_MAX_TEXT_LENGTH);
    display_text[LCD_MAX_TEXT_LENGTH] = '\0';

    int len = (int)strlen(display_text);

    /* Clear the text rendering region (avoid alarm indicator at 127,0) */
    for (int y = TEXT_START_Y; y < TEXT_START_Y + FONT_CHAR_HEIGHT; y++)
        for (int x = 0; x < LCD_DISPLAY_WIDTH - 1; x++)
            set_lcd_pixel(x, y, 0);

    /* Center text horizontally: each char is FONT_CHAR_WIDTH + 1px spacing */
    int total_width = len * (FONT_CHAR_WIDTH + 1) - 1;
    int cursor_x = (LCD_DISPLAY_WIDTH - total_width) / 2;

    for (int i = 0; i < len; i++) {
        const uint8_t *glyph = font5x7_get_glyph(display_text[i]);
        for (int col = 0; col < FONT_CHAR_WIDTH; col++) {
            uint8_t column_data = glyph[col];
            for (int row = 0; row < FONT_CHAR_HEIGHT; row++) {
                if (column_data & (1 << row))
                    set_lcd_pixel(cursor_x + col, TEXT_START_Y + row, 1);
            }
        }
        cursor_x += FONT_CHAR_WIDTH + 1;
    }
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