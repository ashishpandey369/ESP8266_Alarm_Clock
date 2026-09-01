#pragma once

#include <Arduino.h>

class Button {
public:
    enum class Event {
        None,
        ShortPress,
        LongPress
    };

    explicit Button(uint8_t pin);

    void begin();
    bool pressed();
    Event event(uint32_t longPressMs = 3000);
    bool repeatPressed(uint32_t initialDelayMs = 500, uint32_t repeatIntervalMs = 100);

private:
    uint8_t pin_;
    bool lastState_;
    bool stableState_;
    uint32_t lastChangeMs_;
    uint32_t pressStartMs_;
    uint32_t lastRepeatMs_;
    bool longPressReported_;
};
