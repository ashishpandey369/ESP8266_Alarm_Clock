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

// TM1637 4-digit display
constexpr uint8_t DISPLAY_CLK = 5;   // GPIO5 / D1
constexpr uint8_t DISPLAY_DIO = 4;   // GPIO4 / D2

// DS3231 RTC - I2C bus
constexpr uint8_t RTC_SDA = 4;       // GPIO4 / D2
constexpr uint8_t RTC_SCL = 5;       // GPIO5 / D1

// Control buttons: GPIO -> button -> GND; use INPUT_PULLUP
constexpr uint8_t BUTTON_SET  = 12;  // GPIO12 / D6
constexpr uint8_t BUTTON_UP   = 13;  // GPIO13 / D7
constexpr uint8_t BUTTON_DOWN = 14;  // GPIO14 / D5

// Alarm output: GPIO -> base resistor -> NPN transistor -> buzzer
constexpr uint8_t BUZZER = 16;       // GPIO16 / D0

} // namespace Pins

// NOTE: TM1637 and DS3231 use different protocols, so their signals
// must NOT share the same physical GPIO pins. The RTC mapping above
// is provisional until the exact display/module wiring is finalized.
// ESP-12F boot pins: GPIO0 HIGH, GPIO2 HIGH, GPIO15 LOW during normal boot.
