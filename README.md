# Hatch Takehome Project
Targetted to the C99 standard as the lowest common denominator for simplicity.

The frame buffer is the only major memory hog. It's a packed (1bpp) array, currently set to 128x32, so it uses 4k of memory. If the LCD doesn't use a full framebuffer (i.e. text writing), it wouldn't necessary. We could also write pixel by pixel direct to the LCD and not need the internal buffer, but that could be slow depending on the hardware.

Here, buttons are simple variables with get functions (and set functions for testing). The functions could easily be wired up to GPIO though.

The only code written by AI is the framebuffer and font rendering. Mostly AI was just used to define the font bitmaps.

## Design

### loop.c
The loop function is the periodic callback used by an RTOS, a loop in bare metal, or a standalone app like here. It uses the system state to determine what is displayed. It is a very simple implementation of the requirements and just sets the display based on buttons or the state. After a button is pressed, a timer is used to keep the hold the display for two seconds.

```c
    void loop();
```

### lcd_display.c
The LCD display has a frame buffer, but also stores a text string when text is written to it. Since some LCD displays allow direct text, and others are pure pixels, I added support for either.

If the libpng driver is available when CMake is used, it comiles in PNG writing, so you can dump the contents of the frame for debugging. 

### font.c
The code for font rendering to the frame buffer is written by AI, it defines some simple fonts and can burn them in to the frame.

## Building

```bash
    mkdir build
    cd build
    cmake ..
    make
```

## Tests

A simple unit test app is included with this project. It uses the same code as the main app, except for test.c instead of main.c.

The tests work by setting system states, calling the ```loop()``` function, and then reading the LCD. If the LCD doesn't have the expected values, an assert fails and the program exits.

If the PNG library was found when running CMake, PNG files will be created with the contents of the framebuffer. This could be used as part of a regression test by comparing reference images with the images produced by the test.

### Test Stage Images

**1. Clock with no alarm set**

![Clock no alarm](images/test_01_clock_no_alarm.png)

**2. Single digit button pressed**

![Single digit](images/test_02_single_digit.png)

**3. Display hold active (button released, still showing digit)**

![Hold active](images/test_03_hold_active.png)

**4. Clock returns after hold expires**

![Clock after hold](images/test_04_clock_after_hold.png)

**5. Alarm button pressed, no alarm set (--:--)**

![No alarm set](images/test_05_alarm_no_alarm_set.png)

**6. Alarm button pressed, alarm set (shows alarm time)**

![Alarm time set](images/test_06_alarm_time_set.png)

**7. Clock with alarm indicator dot on**

![Alarm dot on](images/test_07_clock_alarm_dot_on.png)

### Running the Tests

From the build directory, run:

```bash
./display_controller_test
```

You should get output like this:

```
Alarm indicator state: 0
Clock LCD text: 15:55
Single digit button LCD Text: '  1  '
LCD Text should still be the same as above: '  1  '
Clock LCD text: 15:55
Alarm time with no alarm set LCD Text: --:--
Clock LCD text: 15:55
Alarm indicator state: 1
ALL TESTS PASSED!
```
