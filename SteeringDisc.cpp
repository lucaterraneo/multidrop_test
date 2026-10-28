#include "SteeringDisc.h"
#include "Config.h"

SteeringDisc::SteeringDisc(
    DigitalStSolidExpansion* expansion,
    int ssr,
    int ledSsr
) {
    this->expansion = expansion;
    this->ssr = ssr;
    this->ledSsr = ledSsr;

    running = false;
    solenoidOn = false;
    lastToggleMs = 0;
}

void SteeringDisc::writeValve() {
    // SSR HIGH applies 24 V to the coil.
    expansion->digitalWrite(ssr, solenoidOn ? HIGH : LOW);
    expansion->updateDigitalOutputs();
}

void SteeringDisc::writeLed() {
    // Same SSR pattern, driving the button's LED instead of a coil.
    expansion->digitalWrite(ledSsr, running ? HIGH : LOW);
    expansion->updateDigitalOutputs();
}

void SteeringDisc::begin() {
    solenoidOn = false;
    writeValve();
    writeLed();
}

unsigned long SteeringDisc::intervalMs() const {
    if (!Config::USE_POT) {
        return Config::Timing::DISC_INTERVAL_MS;
    }

    int potValue = analogRead(Config::POT_PIN);
    return map(
        potValue,
        0,
        1023,
        Config::POT_INTERVAL_MIN_MS,
        Config::POT_INTERVAL_MAX_MS
    );
}

void SteeringDisc::runCycle() {
    if (!running) {
        return;
    }

    unsigned long now = millis();
    if (now - lastToggleMs < intervalMs()) {
        return;
    }

    solenoidOn = !solenoidOn;
    writeValve();
    lastToggleMs = now;
}

void SteeringDisc::start() {
    running = true;
    solenoidOn = true;
    writeValve();
    lastToggleMs = millis();
    writeLed();
}

void SteeringDisc::stop() {
    running = false;
    solenoidOn = false;
    writeValve();
    writeLed();
}

void SteeringDisc::update() {
    runCycle();
}

bool SteeringDisc::isRunning() const {
    return running;
}
