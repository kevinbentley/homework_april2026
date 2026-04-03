cmake_minimum_required(VERSION 3.10)
project(display_controller LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)

add_executable(display_controller
    src/main.c
    src/font5x7.c
    src/lcd_display.c
    src/utils.c
    src/alarm.c
    src/buttons.c
    src/loop.c
)

target_include_directories(display_controller PRIVATE include)

add_executable(display_controller_test
    src/test.c
    src/font5x7.c
    src/lcd_display.c
    src/utils.c
    src/alarm.c
    src/buttons.c
    src/loop.c
)

target_include_directories(display_controller_test PRIVATE include)

add_executable(display_controller_tui
    src/tui_main.c
    src/font5x7.c
    src/lcd_display.c
    src/utils.c
    src/alarm.c
    src/buttons.c
    src/loop.c
)

target_include_directories(display_controller_tui PRIVATE include)
