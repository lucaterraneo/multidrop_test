#include "Dropper.h"
#include "Config.h"

Dropper::Dropper(
    DigitalStSolidExpansion* expansion,
    int ssr,
    int switchPin,
    int startPin,
    int stopPin,
    int greenPin,
    int redPin
) {
    this->expansion = expansion;
    this->ssr = ssr;
    this->switchPin = switchPin;
    this->startPin = startPin;
    this->stopPin = stopPin;
    this->greenPin = greenPin;
    this->redPin = redPin;

    running = false;
    solenoidOn = false;
    lastSwitchBottom = false;
    lastStart = LOW;
    lastStop = LOW;
    lastToggleMs = 0;
}

void Dropper::writeValve() {
    // SSR HIGH applies 24 V to the coil. That command is the bottom position.
    expansion->digitalWrite(ssr, solenoidOn ? HIGH : LOW);
    expansion->updateDigitalOutputs();
}

void Dropper::writeLeds() {
    digitalWrite(greenPin, running ? HIGH : LOW);
    digitalWrite(redPin, running ? LOW : HIGH);
}

void Dropper::begin() {
    pinMode(switchPin, INPUT);
    pinMode(startPin, INPUT);
    pinMode(stopPin, INPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);

    lastSwitchBottom = digitalRead(switchPin) == HIGH;
    solenoidOn = lastSwitchBottom;
    writeValve();
    writeLeds();
}

void Dropper::handleButtons() {
    bool startNow = digitalRead(startPin);
    bool stopNow = digitalRead(stopPin);

    // 24 V on the input is a press.
    if (lastStart == LOW && startNow == HIGH) {
        running = true;
        lastToggleMs = millis();
    }

    if (lastStop == LOW && stopNow == HIGH) {
        running = false;
        solenoidOn = true;
        writeValve();
        lastSwitchBottom = digitalRead(switchPin) == HIGH;
    }

    lastStart = startNow;
    lastStop = stopNow;
    writeLeds();
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

void Dropper::update() {
    handleButtons();
    runCycle();
    followSwitch();
}

bool Dropper::isRunning() const {
    return running;
}
