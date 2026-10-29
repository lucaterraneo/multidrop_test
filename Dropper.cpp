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
    lastSwitchUp = false;
    lastToggleMs = 0;
}

void Dropper::writeValve() {
    // SSR HIGH applies 24 V to the coil. That command drives the dropper up.
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

    lastSwitchUp = digitalRead(switchPin) == HIGH;
    solenoidOn = lastSwitchUp;
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

    bool switchUp = digitalRead(switchPin) == HIGH;
    if (switchUp == lastSwitchUp) {
        return;
    }

    lastSwitchUp = switchUp;
    solenoidOn = switchUp;
    writeValve();
    Serial.println(switchUp ? "dropper switch: UP" : "dropper switch: DOWN");
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
    lastSwitchUp = digitalRead(switchPin) == HIGH;
    writeLed();
}

void Dropper::update(bool inhibited) {
    runCycle();

    if (inhibited) {
        return;
    }

    followSwitch();
}

bool Dropper::isRunning() const {
    return running;
}
