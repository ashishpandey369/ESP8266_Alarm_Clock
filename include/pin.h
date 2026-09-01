#pragma once

#include <Arduino.h>

// ============================================================
// ESP8266 ESP-12F - Hardware Pin Map
// ============================================================
// Keep ALL GPIO assignments in this file.
// Other modules should use Pins::<name> instead of hard-coding
// GPIO numbers. If the hardware wiring changes, change it here.
// ============================================================

namespace Pins {

// -------------------- DS1307 RTC -----------------------------
// I2C interface. SDA/SCL are dedicated to the RTC.
constexpr uint8_t RTC_SDA = 4;       // GPIO4 / D2
constexpr uint8_t RTC_SCL = 5;       // GPIO5 / D1

// -------------------- TM1637 Display -------------------------
// TM1637 uses a separate 2-wire protocol, so it uses GPIOs
// different from the DS1307 I2C pins.
constexpr uint8_t DISPLAY_CLK = 12;  // GPIO12 / D6
constexpr uint8_t DISPLAY_DIO = 13;  // GPIO13 / D7

// -------------------- Control Buttons ------------------------
// Recommended wiring: GPIO -> button -> GND; use INPUT_PULLUP.
// GPIO0 and GPIO2 are boot-sensitive; verify final button wiring
// so they are not held LOW while the ESP-12F is booting.
constexpr uint8_t BUTTON_SET   = 14; // GPIO14 / D5
constexpr uint8_t BUTTON_UP    = 0;  // GPIO0  / D3
constexpr uint8_t BUTTON_DOWN  = 2;  // GPIO2  / D4

// -------------------- Alarm Buzzer ---------------------------
// GPIO drives the base resistor of the NPN transistor stage.
constexpr uint8_t BUZZER = 16;       // GPIO16 / D0

} // namespace Pins

// ============================================================
// DS1307 MODULE CONNECTIONS
// ============================================================
// VCC -> approved supply voltage for the exact RTC module
// GND -> common GND
// SDA -> GPIO4 / D2
// SCL -> GPIO5 / D1
// BAT -> RTC backup cell using the correct battery type
// SQ  -> NC for this project
// DS  -> NC for this project unless the exact board requires it
//
// Do NOT connect BAT directly to 3.3V.
// ============================================================

// ============================================================
// ESP-12F BOOT PINS
// ============================================================
// GPIO0 must be HIGH during normal boot and LOW for flashing.
// GPIO2 must be HIGH during normal boot.
// GPIO15 must be LOW during normal boot.
// ============================================================
