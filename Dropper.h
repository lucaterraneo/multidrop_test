#ifndef DROPPER_H
#define DROPPER_H

#include <Arduino.h>
#include "OptaBlue.h"

using namespace Opta;

// M1 dropper. When stopped, the valve follows the position switch.
class Dropper {

private:

    DigitalStSolidExpansion* expansion;
    int ssr;
    int switchPin;
    int startPin;
    int stopPin;
    int greenPin;
    int redPin;

    bool running;
    bool solenoidOn;
    bool lastSwitchBottom;
    bool lastStart;
    bool lastStop;
    unsigned long lastToggleMs;

    void writeValve();
    void writeLeds();
    void handleButtons();
    void runCycle();
    void followSwitch();

public:

    Dropper(
        DigitalStSolidExpansion* expansion,
        int ssr,
        int switchPin,
        int startPin,
        int stopPin,
        int greenPin,
        int redPin
    );

    void begin();
    void update();
    bool isRunning() const;
};

#endif
