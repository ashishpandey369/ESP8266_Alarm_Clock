#pragma once

// ESP8266 ESP-12F GPIO assignments.
// Keep all physical pin mappings in this file.

namespace Pins {

// TM1637 4-digit display
constexpr uint8_t DISPLAY_CLK = 5;   // GPIO5 / D1
constexpr uint8_t DISPLAY_DIO = 4;   // GPIO4 / D2

// User interface buttons
constexpr uint8_t BUTTON_SET  = 12; // GPIO12 / D6
constexpr uint8_t BUTTON_UP   = 13; // GPIO13 / D7
constexpr uint8_t BUTTON_DOWN = 14; // GPIO14 / D5

// Alarm output
constexpr uint8_t BUZZER = 16;      // GPIO16 / D0

} // namespace Pins
