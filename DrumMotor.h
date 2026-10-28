#ifndef DRUM_MOTOR_H
#define DRUM_MOTOR_H

#include <Arduino.h>

class DrumMotor {

private:

    int switchPin;
    bool on;
    float revolutions;
    unsigned long lastMs;
    unsigned long lastReportMs;
    unsigned long runStartMs;

    void accrue();
    void reset();
    void report(const char* status,
                unsigned long elapsedMs) const;

    void printDuration(unsigned long ms) const;

public:

    explicit DrumMotor(int switchPin);
    void begin();
    void update();
    bool isOn() const;
    float drumRevolutions() const;
    float drumKilometres() const;
};

#endif

