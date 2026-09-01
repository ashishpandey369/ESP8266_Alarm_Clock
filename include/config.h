#pragma once

#include <Arduino.h>

namespace Config {

// Display
constexpr uint8_t DEFAULT_BRIGHTNESS = 7;

// Alarm defaults
constexpr uint8_t DEFAULT_ALARM_HOUR = 7;
constexpr uint8_t DEFAULT_ALARM_MINUTE = 0;
bool alarmEnabled = false;

// Time settings
constexpr long UTC_OFFSET_SECONDS = 19800; // IST (UTC+05:30)
constexpr int DAYLIGHT_OFFSET_SECONDS = 0;

// Wi-Fi placeholders. These will be moved to a safer local configuration later.
constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

} // namespace Config
