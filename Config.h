#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================
//  Change the parameters here. No need to touch the other files.
//  SSR indices: 0 = first solid-state relay of the D1608S.
//  Opta inputs: I1 = A0 ... I8 = A7. A pressed button puts 24 V on the input.
// ============================================================

namespace Config {

    namespace Pins {

        // HIGH = dropper at the bottom, LOW = at the top
        constexpr int DROP_SWITCH   = A0;  // I1

        // External 24 V illuminated NO buttons. HIGH = pressed.
        constexpr int DROPPER_BUTTON = A1;  // I2
        constexpr int DISC_BUTTON   = A2;  // I3

        // Feed-through: same 24 V that feeds the contactor coil.
        constexpr int MOTOR_SWITCH  = A6;  // I7

        // D1608S solid-state relays, 24 V straight to the valve coil.
        constexpr int DROPPER_SSR     = 0;  // O1
        constexpr int DISC_SSR        = 1;  // O2

        // Same expansion, driving each button's built-in LED (through its
        // own series resistor). On = lit while that program is running.
        constexpr int DROPPER_LED_SSR = 2;  // O3
        constexpr int DISC_LED_SSR    = 3;  // O4
    }

    namespace Timing {

        constexpr unsigned long DROPPER_INTERVAL_MS = 1000;
        constexpr unsigned long DISC_INTERVAL_MS    = 1000;
        constexpr unsigned long LOOP_DELAY_MS       = 20;
        constexpr unsigned long MOTOR_REPORT_MS     = 1000;
        constexpr unsigned long BUTTON_DEBOUNCE_MS  = 40;
    }

    // Drum speed from I7 on-time. No encoder: 1500 U/min / 20:1 = 75 rpm.
    namespace Motor {

        constexpr float RPM            = 1500.0f;
        constexpr float GEAR_RATIO     = 20.0f;
        constexpr float DRUM_DIAMETER_MM = 250.0f;
        constexpr float DRUM_RPM       = RPM / GEAR_RATIO;
    }

    // Optional pot for the steering-disc interval. Off unless you wire it to I6.
    constexpr bool USE_POT = false;
    constexpr int POT_PIN = A5;
    constexpr unsigned long POT_INTERVAL_MIN_MS = 500;
    constexpr unsigned long POT_INTERVAL_MAX_MS = 4000;

}

#endif
