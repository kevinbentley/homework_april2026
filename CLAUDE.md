# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

An application-level C module for controlling a dot-matrix display on an RTOS-based embedded device (Hatch take-home assignment). Written to the C99 standard. Hardware drivers and RTOS primitives are mocked; the module is designed to slot into a larger firmware project.

## Build and Test

```bash
# Build (from repo root)
mkdir -p build && cd build
cmake .. && make

# Run tests
./build/display_controller_test

# Run the app (loops forever, Ctrl-C to stop)
./build/display_controller
```

Both targets share all source files except the entry point: `main.c` (app) vs `test.c` (tests).

## Architecture

The system is a 10ms periodic loop (`loop()`) that reads button state, checks alarm status, and writes to the LCD. There is no event system or callbacks — everything is polled each tick.

### Display hold mechanism

When a button is pressed, `loop()` records a future timestamp (`time_until_clock_displayed = now + DISPLAY_HOLD_SECONDS`). While the hold is active, the display keeps showing the button's content even after the button is released. Once the hold expires, the display reverts to the clock. A new button press while a hold is active is ignored (early return).

### Module responsibilities

- **loop.c** — The only orchestrator. Reads buttons, formats display content, writes LCD. All display logic lives here.
- **lcd_display.c/h** — Mock LCD driver. Text buffer (`set_lcd_text`/`get_lcd_text`) and a bit-packed framebuffer for individual pixels. The alarm indicator is a single pixel at `(LCD_DISPLAY_WIDTH-1, 0)`.
- **buttons.c/h** — Two buttons: alarm-time and single-digit. Getters poll state; setters exist only for testing (would be GPIO/ISR in production).
- **alarm.c/h** — Stores alarm time as `time_t`. `is_alarm_set()` returns whether alarm != 0.
- **utils.c/h** — `get_hhmm_time_string()` formats `time_t` to "HH:MM"; `sleep_ms()` wraps `nanosleep`.

### Key constants (in lcd_display.h)

- `DISPLAY_HOLD_SECONDS` = 2
- `LCD_DISPLAY_WIDTH` = 128, `LCD_DISPLAY_HEIGHT` = 32
- `LCD_MAX_TEXT_LENGTH` = 5

## Testing approach

Tests in `test.c` are sequential assertions — set system state, call `loop()`, read LCD text/pixels, assert expected values. Tests manipulate time by calling `sleep()` to wait out the display hold period. There is no test framework; failures are bare `assert()` calls.
