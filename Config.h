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

        constexpr int DROPPER_START = A1;  // I2
        constexpr int DROPPER_STOP  = A2;  // I3
        constexpr int DISC_START    = A3;  // I4
        constexpr int DISC_STOP     = A4;  // I5

        // On-board Opta LEDs. HIGH = lit.
        constexpr int DROPPER_GREEN = LED_D0;
        constexpr int DROPPER_RED   = LED_D1;
        constexpr int DISC_GREEN    = LED_D2;
        constexpr int DISC_RED      = LED_D3;

        // D1608S solid-state relays, 24 V straight to the valve coil.
        constexpr int DROPPER_SSR   = 0;
        constexpr int DISC_SSR      = 1;
    }

    namespace Timing {

        constexpr unsigned long DROPPER_INTERVAL_MS = 2500;
        constexpr unsigned long DISC_INTERVAL_MS    = 2000;
        constexpr unsigned long LOOP_DELAY_MS       = 20;
    }

    // Optional pot for the steering-disc interval. Off unless you wire it to I6.
    constexpr bool USE_POT = false;
    constexpr int POT_PIN = A5;
    constexpr unsigned long POT_INTERVAL_MIN_MS = 500;
    constexpr unsigned long POT_INTERVAL_MAX_MS = 4000;

}

#endif
