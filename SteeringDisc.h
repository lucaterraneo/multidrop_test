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
    int ledSsr;

    bool running;
    bool solenoidOn;
    unsigned long lastToggleMs;
    long lastReportedIntervalMs;

    void writeValve();
    void writeLed();
    void runCycle();
    void reportPotIfChanged();
    unsigned long intervalMs() const;

public:

    SteeringDisc(
        DigitalStSolidExpansion* expansion,
        int ssr,
        int ledSsr
    );

    void begin();
    void update();
    void start();
    void stop();
    bool isRunning() const;
};

#endif
