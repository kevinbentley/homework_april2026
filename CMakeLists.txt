cmake_minimum_required(VERSION 3.10)
project(display_controller LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)

add_executable(display_controller
    src/main.c
    src/lcd_display.c
    src/utils.c
    src/alarm.c
    src/buttons.c
    src/loop.c
    src/font.c
)

target_include_directories(display_controller PRIVATE include)

add_executable(display_controller_test
    src/test.c
    src/lcd_display.c
    src/utils.c
    src/alarm.c
    src/buttons.c
    src/loop.c
    src/font.c
)

target_include_directories(display_controller_test PRIVATE include)

find_package(PNG QUIET)
if(PNG_FOUND)
    target_link_libraries(display_controller_test PNG::PNG)
    target_compile_definitions(display_controller_test PRIVATE HAVE_PNG)
    message(STATUS "libpng found — PNG framebuffer snapshots enabled in tests")
else()
    message(STATUS "libpng not found — PNG snapshots disabled")
endif()