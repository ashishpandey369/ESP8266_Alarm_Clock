#pragma once

#include <Arduino.h>

class AlarmManager {
public:
    AlarmManager();

    void begin(uint8_t defaultHour, uint8_t defaultMinute);
    void setTime(uint8_t hour, uint8_t minute);
    uint8_t getHour() const;
    uint8_t getMinute() const;
    void enable();
    void disable();
    bool isEnabled() const;
    bool matches(uint8_t hour, uint8_t minute) const;

private:
    void save() const;

    uint8_t hour_;
    uint8_t minute_;
    bool enabled_;
};
