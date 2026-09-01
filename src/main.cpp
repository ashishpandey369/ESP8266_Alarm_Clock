#include <Arduino.h>
#include "pin.h"
#include "display.h"

ClockDisplay clockDisplay(Pins::DISPLAY_CLK, Pins::DISPLAY_DIO);

namespace {
constexpr unsigned long COLON_BLINK_INTERVAL_MS = 1000;
unsigned long lastColonToggle = 0;
bool colonVisible = true;
}

void setup() {
    Serial.begin(115200);
    delay(100);

    pinMode(Pins::BUTTON_SET, INPUT_PULLUP);
    pinMode(Pins::BUTTON_UP, INPUT_PULLUP);
    pinMode(Pins::BUTTON_DOWN, INPUT_PULLUP);
    pinMode(Pins::BUZZER, OUTPUT);

    digitalWrite(Pins::BUZZER, LOW);

    // Initialize the TM1637 and show a fixed test value.
    // The center colon will blink once per second.
    clockDisplay.begin();
    clockDisplay.showTime(12, 34, colonVisible);

    Serial.println();
    Serial.println("ESP8266 Alarm Clock - hardware bring-up");
    Serial.println("TM1637 display initialized.");
    Serial.println("Display test: 12:34");
    Serial.println("Colon blink: 1 second interval");
}

void loop() {
    const unsigned long now = millis();

    if (now - lastColonToggle >= COLON_BLINK_INTERVAL_MS) {
        lastColonToggle = now;
        colonVisible = !colonVisible;
        clockDisplay.showTime(12, 34, colonVisible);
    }
}
