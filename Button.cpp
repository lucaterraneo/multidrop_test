#include "Button.h"
#include "Config.h"

Button::Button(int pin) {
    this->pin = pin;
    lastRead = false;
    stablePressed = false;
    lastChangeMs = 0;
}

void Button::begin() {
    pinMode(pin, INPUT);
    lastRead = digitalRead(pin) == HIGH;
    stablePressed = lastRead;
    lastChangeMs = millis();
}

bool Button::pressed() {
    bool now = digitalRead(pin) == HIGH;
    unsigned long nowMs = millis();

    if (now != lastRead) {
        lastRead = now;
        lastChangeMs = nowMs;
    }

    if ((nowMs - lastChangeMs) >= Config::Timing::BUTTON_DEBOUNCE_MS && now != stablePressed) {
        stablePressed = now;
        return stablePressed;
    }

    return false;
}
