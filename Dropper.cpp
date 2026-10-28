#include "Dropper.h"
#include "Config.h"

Dropper::Dropper(
    DigitalStSolidExpansion* expansion,
    int ssr,
    int switchPin,
    int ledSsr
) {
    this->expansion = expansion;
    this->ssr = ssr;
    this->switchPin = switchPin;
    this->ledSsr = ledSsr;

    running = false;
    solenoidOn = false;
    lastSwitchBottom = false;
    lastToggleMs = 0;
}

void Dropper::writeValve() {
    // SSR HIGH applies 24 V to the coil. That command is the bottom position.
    expansion->digitalWrite(ssr, solenoidOn ? HIGH : LOW);
    expansion->updateDigitalOutputs();
}

void Dropper::writeLed() {
    // Same SSR pattern, driving the button's LED instead of a coil.
    expansion->digitalWrite(ledSsr, running ? HIGH : LOW);
    expansion->updateDigitalOutputs();
}

void Dropper::begin() {
    pinMode(switchPin, INPUT);

    lastSwitchBottom = digitalRead(switchPin) == HIGH;
    solenoidOn = lastSwitchBottom;
    writeValve();
    writeLed();
}

void Dropper::runCycle() {
    if (!running) {
        return;
    }

    unsigned long now = millis();
    if (now - lastToggleMs < Config::Timing::DROPPER_INTERVAL_MS) {
        return;
    }

    solenoidOn = !solenoidOn;
    writeValve();
    lastToggleMs = now;
}

void Dropper::followSwitch() {
    if (running) {
        return;
    }

    bool switchBottom = digitalRead(switchPin) == HIGH;
    if (switchBottom == lastSwitchBottom) {
        return;
    }

    lastSwitchBottom = switchBottom;
    solenoidOn = switchBottom;
    writeValve();
}

void Dropper::start() {
    running = true;
    solenoidOn = true;
    writeValve();
    lastToggleMs = millis();
    writeLed();
}

void Dropper::stop() {
    running = false;
    solenoidOn = true;
    writeValve();
    lastSwitchBottom = digitalRead(switchPin) == HIGH;
    writeLed();
}

void Dropper::update() {
    runCycle();
    followSwitch();
}

bool Dropper::isRunning() const {
    return running;
}
