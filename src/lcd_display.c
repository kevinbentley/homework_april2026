#include "lcd_display.h"
#include "font.h"
#include <string.h>

static char display_text[LCD_MAX_TEXT_LENGTH+1] = "";
/* The +7 makes sure we round up for room if we have an odd size */
uint8_t framebuffer[(LCD_DISPLAY_WIDTH * LCD_DISPLAY_HEIGHT + 7) / 8];

/* 
    Even though we have a full framebuffer, this code assumes there's a 'write text' 
    part of the driver 
*/
void set_lcd_text(const char *text)
{
    /* 
        Calculate start pixel for centering 5 characters 
        Characters are 8 pixels wide, plus one space. Centered around 5 chars
        16 pixels high.
    */
    const uint32_t x = (LCD_DISPLAY_WIDTH/2) - (9*2.5);
    const uint32_t y = (LCD_DISPLAY_HEIGHT/2) - (8);
    strncpy(display_text,text,LCD_MAX_TEXT_LENGTH);
    /* Some call to a driver that writes to text goes here. */
    //render_text()
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

#ifdef HAVE_PNG
#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#define PNG_SCALE  4
#define TEXT_X     42   /* (128 - 5*8 - 4*1) / 2 */
#define TEXT_Y     8    /* (32 - 16) / 2 */

void save_lcd_png(const char *filename)
{
    /* Snapshot alarm dot, clear, re-render everything for a clean frame */
    int alarm_dot = get_lcd_pixel(LCD_DISPLAY_WIDTH - 1, 0);
    framebuffer_clear();
    set_lcd_pixel(LCD_DISPLAY_WIDTH - 1, 0, alarm_dot);
    render_text(TEXT_X, TEXT_Y, display_text, 1);

    int w = LCD_DISPLAY_WIDTH * PNG_SCALE;
    int h = LCD_DISPLAY_HEIGHT * PNG_SCALE;

    FILE *fp = fopen(filename, "wb");
    if (!fp) return;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) { fclose(fp); return; }

    png_infop info = png_create_info_struct(png);
    if (!info) { png_destroy_write_struct(&png, NULL); fclose(fp); return; }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, w, h, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    uint8_t *row = malloc(w * 3);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int on = get_lcd_pixel(x / PNG_SCALE, y / PNG_SCALE);
            row[x * 3 + 0] = 0;
            row[x * 3 + 1] = on ? 200 : 20;  /* green-on-dark dot-matrix look */
            row[x * 3 + 2] = 0;
        }
        png_write_row(png, row);
    }

    png_write_end(png, NULL);
    free(row);
    png_destroy_write_struct(&png, &info);
    fclose(fp);

    /* Restore framebuffer to only the alarm dot so pixel checks still work */
    framebuffer_clear();
    set_lcd_pixel(LCD_DISPLAY_WIDTH - 1, 0, alarm_dot);
}
#endif