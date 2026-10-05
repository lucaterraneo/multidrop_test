#include "OptaBlue.h"
#include "Config.h"
#include "Dropper.h"
#include "SteeringDisc.h"

using namespace Opta;

DigitalStSolidExpansion expansion;

Dropper dropper(
    &expansion,
    Config::Pins::DROPPER_SSR,
    Config::Pins::DROP_SWITCH,
    Config::Pins::DROPPER_START,
    Config::Pins::DROPPER_STOP,
    Config::Pins::DROPPER_GREEN,
    Config::Pins::DROPPER_RED
);

SteeringDisc steeringDisc(
    &expansion,
    Config::Pins::DISC_SSR,
    Config::Pins::DISC_START,
    Config::Pins::DISC_STOP,
    Config::Pins::DISC_GREEN,
    Config::Pins::DISC_RED
);

void setup() {
    OptaController.begin();
    expansion = OptaController.getExpansion(0);

    dropper.begin();
    steeringDisc.begin();
}

void loop() {
    dropper.update();
    steeringDisc.update();
    delay(Config::Timing::LOOP_DELAY_MS);
}
