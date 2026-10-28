#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// External 24 V NO button (Opta digital input). HIGH = pressed.
class Button {

private:

    int pin;
    bool lastRead;
    bool stablePressed;
    unsigned long lastChangeMs;

public:

    explicit Button(int pin);

    void begin();
    // true exactly once, on the debounced press edge.
    bool pressed();
};

#endif
