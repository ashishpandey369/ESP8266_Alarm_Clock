#pragma once

#include <Arduino.h>

class Button {
public:
    explicit Button(uint8_t pin);

    void begin();
    bool pressed();

private:
    uint8_t pin_;
    bool lastState_;
    bool stableState_;
    uint32_t lastChangeMs_;
};
