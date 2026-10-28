#include "DrumMotor.h"
#include "Config.h"

DrumMotor::DrumMotor(int switchPin) {

    this->switchPin = switchPin;
    on = false;
    revolutions = 0.0f;

    lastMs = 0;
    lastReportMs = 0;
    runStartMs = 0;
}


// =====================================================
// ACCRUE REVOLUTIONS
// =====================================================
void DrumMotor::accrue() {

    unsigned long now = millis();
    unsigned long dt = now - lastMs;
    lastMs = now;
    revolutions += (dt / 60000.0f) * Config::Motor::DRUM_RPM;
}


// =====================================================
// RESET SESSION
// =====================================================
void DrumMotor::reset() {

    revolutions = 0.0f;

    lastMs = millis();
    lastReportMs = lastMs;
    runStartMs = lastMs;
}


// =====================================================
// PRINT HH:MM:SS
// =====================================================
void DrumMotor::printDuration(unsigned long ms) const {

    unsigned long totalSeconds = ms / 1000UL;

    unsigned long hours = totalSeconds / 3600UL;
    unsigned long minutes = (totalSeconds % 3600UL) / 60UL;
    unsigned long seconds = totalSeconds % 60UL;

    if (hours < 10) Serial.print('0');
    Serial.print(hours);
    Serial.print(':');

    if (minutes < 10) Serial.print('0');
    Serial.print(minutes);
    Serial.print(':');

    if (seconds < 10) Serial.print('0');
    Serial.print(seconds);
}


// =====================================================
// REPORT on Serial Monitor
// =====================================================
void DrumMotor::report(
    const char* status,
    unsigned long elapsedMs
) const {

    Serial.print("Rotations: ");
    Serial.print(revolutions, 2);

    Serial.print(" | km: ");
    Serial.print(drumKilometres(), 5);

    Serial.print(" | Start since boot: ");
    printDuration(runStartMs);

    Serial.print(" | Running for: ");
    printDuration(elapsedMs);

    Serial.print(" | Status: ");
    Serial.println(status);
}


// =====================================================
// BEGIN
// =====================================================
void DrumMotor::begin() {

    pinMode(switchPin, INPUT);

    on = false;

    reset();

    if (digitalRead(switchPin) == HIGH) {

        on = true;

        report("RUNNING", 0);
    }
}


// =====================================================
// UPDATE
// =====================================================
void DrumMotor::update() {

    bool switchOn = digitalRead(switchPin) == HIGH;

    unsigned long now = millis();


    // -----------------------------------------
    // OFF -> ON
    // -----------------------------------------
    if (switchOn && !on) {

        reset();

        on = true;

        Serial.println("MOTOR START");

        report("RUNNING", 0);

        return;
    }


    // -----------------------------------------
    // MOTOR RUNNING
    // -----------------------------------------
    if (switchOn && on) {

        accrue();

        if (now - lastReportMs >= 60000UL) {

            lastReportMs = now;

            report("RUNNING", now - runStartMs);
        }

        return;
    }


    // -----------------------------------------
    // ON -> OFF
    // -----------------------------------------
    if (!switchOn && on) {

        accrue();

        unsigned long duration = now - runStartMs;
        on = false;

        Serial.println("MOTOR END");

        report("END", duration);

        // Keep the final values until the next start.
    }
}


// =====================================================
// STATE
// =====================================================
bool DrumMotor::isOn() const {

    return on;
}


// =====================================================
// REVOLUTIONS
// =====================================================
float DrumMotor::drumRevolutions() const {

    return revolutions;
}


// =====================================================
// KILOMETRES
// =====================================================
float DrumMotor::drumKilometres() const {

    const float diameterM =
        Config::Motor::DRUM_DIAMETER_MM / 1000.0f;

    return revolutions * PI * diameterM / 1000.0f;
}

