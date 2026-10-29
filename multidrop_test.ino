#include "OptaBlue.h"
#include "Config.h"
#include "Dropper.h"
#include "SteeringDisc.h"
#include "DrumMotor.h"
#include "Button.h"

using namespace Opta;

DigitalStSolidExpansion expansion;

Dropper dropper(
    &expansion,
    Config::Pins::DROPPER_SSR,
    Config::Pins::DROP_SWITCH,
    Config::Pins::DROPPER_LED_SSR
);

SteeringDisc steeringDisc(
    &expansion,
    Config::Pins::DISC_SSR,
    Config::Pins::DISC_LED_SSR
);

DrumMotor drumMotor(Config::Pins::MOTOR_SWITCH);
Button dropperButton(Config::Pins::DROPPER_BUTTON);
Button discButton(Config::Pins::DISC_BUTTON);

void stopPneumatics() {
    dropper.stop();
    steeringDisc.stop();
    Serial.println("pneumatics off");
}

void onDropperButton() {
    if (dropper.isRunning() || steeringDisc.isRunning()) {
        stopPneumatics();
        return;
    }

    if (drumMotor.isOn()) {
        Serial.println("dropper blocked: multidrum running");
        return;
    }

    dropper.start();
    Serial.println("dropper on");
}

void onDiscButton() {
    if (dropper.isRunning() || steeringDisc.isRunning()) {
        stopPneumatics();
        return;
    }

    steeringDisc.start();
    Serial.println("back test on");
}

void setup() {
    Serial.begin(115200);
    OptaController.begin();
    expansion = OptaController.getExpansion(0);

    dropper.begin();
    steeringDisc.begin();
    drumMotor.begin();
    dropperButton.begin();
    discButton.begin();
}

void loop() {

    if (dropperButton.pressed()) {
        onDropperButton();
    }
    if (discButton.pressed()) {
        onDiscButton();
    }

    drumMotor.update();

    if (drumMotor.isOn() && dropper.isRunning()) {
        dropper.stop();
        Serial.println("dropper stopped: multidrum running");
    }

    dropper.update(drumMotor.isOn());
    steeringDisc.update();
    delay(Config::Timing::LOOP_DELAY_MS);
}
