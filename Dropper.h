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
    int ledSsr;

    bool running;
    bool solenoidOn;
    bool lastSwitchUp;
    unsigned long lastToggleMs;

    void writeValve();
    void writeLed();
    void runCycle();
    void followSwitch();

public:

    Dropper(
        DigitalStSolidExpansion* expansion,
        int ssr,
        int switchPin,
        int ledSsr
    );

    void begin();
    void update();
    void start();
    void stop();
    bool isRunning() const;
};

#endif
