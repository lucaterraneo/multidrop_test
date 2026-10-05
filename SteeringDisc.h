#ifndef STEERING_DISC_H
#define STEERING_DISC_H

#include <Arduino.h>
#include "OptaBlue.h"

using namespace Opta;

// M2 steering disc. Stop and start both leave the valve off.
class SteeringDisc {

private:

    DigitalStSolidExpansion* expansion;
    int ssr;
    int startPin;
    int stopPin;
    int greenPin;
    int redPin;

    bool running;
    bool solenoidOn;
    bool lastStart;
    bool lastStop;
    unsigned long lastToggleMs;

    void writeValve();
    void writeLeds();
    void handleButtons();
    void runCycle();
    unsigned long intervalMs() const;

public:

    SteeringDisc(
        DigitalStSolidExpansion* expansion,
        int ssr,
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
