#include <Arduino.h>
#include "pin.h"

void setup() {
    Serial.begin(115200);
    delay(100);

    pinMode(Pins::BUTTON_SET, INPUT_PULLUP);
    pinMode(Pins::BUTTON_UP, INPUT_PULLUP);
    pinMode(Pins::BUTTON_DOWN, INPUT_PULLUP);
    pinMode(Pins::BUZZER, OUTPUT);

    digitalWrite(Pins::BUZZER, LOW);

    Serial.println();
    Serial.println("ESP8266 Alarm Clock - hardware bring-up");
    Serial.println("PlatformIO project initialized.");
}

void loop() {
    // Hardware drivers and application modules will be added incrementally.
}
