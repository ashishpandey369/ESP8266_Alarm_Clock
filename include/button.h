#pragma once

#include <Arduino.h>

class Button {
public:
    explicit Button(uint8_t pin);

    void begin();
    bool pressed();
    bool repeatPressed(uint32_t initialDelayMs = 500, uint32_t repeatIntervalMs = 100);

private:
    uint8_t pin_;
    bool lastState_;
    bool stableState_;
    uint32_t lastChangeMs_;
    uint32_t pressStartMs_;
    uint32_t lastRepeatMs_;
};
