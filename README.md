# Hatch Takehome Project
Targetted to the C99 standard as the lowest common denominator for simplicity.

## Assumptions

I assume that the 'real' LCD driver has a framebuffer where individual pixels can be set. 
I also assume it has a 'write text' function of some sort (and read for unit tests).

Here, buttons are simple variables with get functions (and set functions for testing). The functions could easily be wired up to GPIO though.

## Design

### loop.c
The loop function is the periodic callback used by an RTOS, a loop in bare metal, or a standalone app like here. It uses the system state to determine what is displayed.

```c
    void loop();
```

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

### Running the Tests

From the build directory, run:

```bash
./display_controller_test
```

You should get output like this:

```
Alarm indicator state: 0
Clock LCD text: 15:16
Single digit button LCD Text: '  1  '
Alarm time with no alarm set LCD Text: --:--
Alarm indicator state: 1
Alarm LCD text: 15:17
ALL TESTS PASSED!
```