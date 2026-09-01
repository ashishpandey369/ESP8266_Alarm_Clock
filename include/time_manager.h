#pragma once

#include <Arduino.h>

struct ClockTime {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

class TimeManager {
public:
    bool begin();
    bool syncNtp();
    ClockTime now() const;
    bool isSynchronized() const;

private:
    bool synchronized_ = false;
};
