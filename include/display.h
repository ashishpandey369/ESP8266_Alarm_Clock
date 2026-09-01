#pragma once

#include <TM1637Display.h>

class ClockDisplay {
public:
    ClockDisplay(uint8_t clkPin, uint8_t dioPin);

    void begin();
    void showTime(uint8_t hour, uint8_t minute, bool colon = true);
    void showNumber(int value);
    void clear();
    void setBrightness(uint8_t level);

private:
    TM1637Display display_;
};
