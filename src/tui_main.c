#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#include "lcd_display.h"
#include "buttons.h"
#include "alarm.h"
#include "utils.h"

void loop(void);

/* --- Terminal raw mode --------------------------------------------------- */

static struct termios orig_termios;

static void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    printf("\033[?25h");  /* show cursor */
    printf("\n");
}

static void enable_raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* --- Signal handling ----------------------------------------------------- */

static volatile sig_atomic_t running = 1;

static void handle_signal(int sig)
{
    (void)sig;
    running = 0;
}

/* --- Keyboard input ------------------------------------------------------ */

#define BUTTON_HOLD_FRAMES 5  /* hold button ON for 50ms (5 x 10ms ticks) */

static int alarm_btn_frames = 0;
static int digit_btn_frames = 0;

static void poll_keyboard(void)
{
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '1') alarm_btn_frames = BUTTON_HOLD_FRAMES;
        if (c == '2') digit_btn_frames = BUTTON_HOLD_FRAMES;
        if (c == 'q' || c == 'Q') running = 0;
    }

    set_alarm_time_button(alarm_btn_frames > 0 ? BUTTON_ON : BUTTON_OFF);
    set_single_digit_button(digit_btn_frames > 0 ? BUTTON_ON : BUTTON_OFF);

    if (alarm_btn_frames > 0) alarm_btn_frames--;
    if (digit_btn_frames > 0) digit_btn_frames--;
}

/* --- Braille framebuffer renderer ---------------------------------------- */

/*
    Unicode braille characters U+2800..U+28FF encode a 2x4 dot grid:

        col0  col1
    row0  bit0  bit3
    row1  bit1  bit4
    row2  bit2  bit5
    row3  bit6  bit7

    128 / 2 = 64 braille chars wide
    32  / 4 =  8 braille chars tall
*/

static void render_framebuffer(void)
{
    /* Cursor home — overwrites in place, no flicker */
    printf("\033[H");

    printf("  Display Controller TUI\r\n");
    printf("  [1] Alarm Time   [2] Single Digit   [q] Quit\r\n");
    printf("  +-----------------------------------------------------------------+\r\n");

    for (int by = 0; by < LCD_DISPLAY_HEIGHT; by += 4) {
        printf("  |");
        for (int bx = 0; bx < LCD_DISPLAY_WIDTH; bx += 2) {
            unsigned char code = 0;

            if (get_lcd_pixel(bx,     by))     code |= 0x01;
            if (get_lcd_pixel(bx,     by + 1)) code |= 0x02;
            if (get_lcd_pixel(bx,     by + 2)) code |= 0x04;
            if (get_lcd_pixel(bx + 1, by))     code |= 0x08;
            if (get_lcd_pixel(bx + 1, by + 1)) code |= 0x10;
            if (get_lcd_pixel(bx + 1, by + 2)) code |= 0x20;
            if (get_lcd_pixel(bx,     by + 3)) code |= 0x40;
            if (get_lcd_pixel(bx + 1, by + 3)) code |= 0x80;

            /* UTF-8 encode U+2800 + code (always 3 bytes) */
            unsigned int codepoint = 0x2800 + code;
            putchar(0xE0 | ((codepoint >> 12) & 0x0F));
            putchar(0x80 | ((codepoint >>  6) & 0x3F));
            putchar(0x80 | ( codepoint        & 0x3F));
        }
        printf(" |\r\n");
    }

    printf("  +-----------------------------------------------------------------+\r\n");
    printf("  Text: %-6s\r\n", get_lcd_text());
    fflush(stdout);
}

/* --- Main ---------------------------------------------------------------- */

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    signal(SIGINT,  handle_signal);
    signal(SIGTERM, handle_signal);

    enable_raw_mode();
    printf("\033[?25l");  /* hide cursor */
    printf("\033[2J");    /* clear screen */

    framebuffer_clear();

    set_alarm_time(time(NULL)+600);
    time_t clear_time = time(NULL) + 60;

    while (running) {
        poll_keyboard();
        if(clear_time < time(NULL))
            clear_alarm_time();
        loop();
        render_framebuffer();
        sleep_ms(10);
    }

    return 0;
}
