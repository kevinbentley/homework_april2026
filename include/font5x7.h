#ifndef FONT5X7_H_INCLUDED
#define FONT5X7_H_INCLUDED

#include <stdint.h>

#define FONT_CHAR_WIDTH  5
#define FONT_CHAR_HEIGHT 7

/*
    Returns pointer to 5-byte column data for the given ASCII character.
    Each byte represents one column (LSB = top row, bit 6 = bottom row).
    Returns pointer to blank glyph for unsupported characters.
*/
const uint8_t *font5x7_get_glyph(char c);

#endif
