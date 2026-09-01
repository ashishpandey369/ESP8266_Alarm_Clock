#include <Arduino.h>
#include "pin.h"
#include "display.h"

ClockDisplay clockDisplay(Pins::DISPLAY_CLK, Pins::DISPLAY_DIO);

void setup() {
    Serial.begin(115200);
    delay(100);

    pinMode(Pins::BUTTON_SET, INPUT_PULLUP);
    pinMode(Pins::BUTTON_UP, INPUT_PULLUP);
    pinMode(Pins::BUTTON_DOWN, INPUT_PULLUP);
    pinMode(Pins::BUZZER, OUTPUT);

    digitalWrite(Pins::BUZZER, LOW);

    // Initialize the TM1637 and show a fixed test value.
    // This confirms the display wiring before adding RTC/time logic.
    clockDisplay.begin();
    clockDisplay.showTime(12, 34, true);

    Serial.println();
    Serial.println("ESP8266 Alarm Clock - hardware bring-up");
    Serial.println("TM1637 display initialized.");
    Serial.println("Display test: 12:34");
}

void loop() {
    // Keep the display test visible while the remaining hardware modules
    // are brought up incrementally.
}
