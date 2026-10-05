#include "SteeringDisc.h"
#include "Config.h"

SteeringDisc::SteeringDisc(
    DigitalStSolidExpansion* expansion,
    int ssr,
    int startPin,
    int stopPin,
    int greenPin,
    int redPin
) {
    this->expansion = expansion;
    this->ssr = ssr;
    this->startPin = startPin;
    this->stopPin = stopPin;
    this->greenPin = greenPin;
    this->redPin = redPin;

    running = false;
    solenoidOn = false;
    lastStart = LOW;
    lastStop = LOW;
    lastToggleMs = 0;
}

void SteeringDisc::writeValve() {
    // SSR HIGH applies 24 V to the coil.
    expansion->digitalWrite(ssr, solenoidOn ? HIGH : LOW);
    expansion->updateDigitalOutputs();
}

void SteeringDisc::writeLeds() {
    digitalWrite(greenPin, running ? HIGH : LOW);
    digitalWrite(redPin, running ? LOW : HIGH);
}

void SteeringDisc::begin() {
    pinMode(startPin, INPUT);
    pinMode(stopPin, INPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);

    solenoidOn = false;
    writeValve();
    writeLeds();
}

void SteeringDisc::handleButtons() {
    bool startNow = digitalRead(startPin);
    bool stopNow = digitalRead(stopPin);

    // 24 V on the input is a press.
    if (lastStart == LOW && startNow == HIGH) {
        running = true;
        solenoidOn = false;
        writeValve();
        lastToggleMs = millis();
    }

    if (lastStop == LOW && stopNow == HIGH) {
        running = false;
        solenoidOn = false;
        writeValve();
    }

    lastStart = startNow;
    lastStop = stopNow;
    writeLeds();
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

void SteeringDisc::update() {
    handleButtons();
    runCycle();
}

bool SteeringDisc::isRunning() const {
    return running;
}
