#pragma once

#include <Arduino.h>

// DS3231 RTC interface.
// The RTC shares the I2C bus with the TM1637 only if the display is I2C-compatible;
// TM1637 is a separate 2-wire protocol, so its pins remain independent.

class RTCManager {
public:
    bool begin();
    bool isRunning() const;
    bool hasValidTime() const;
    void printStatus(Stream &serial) const;
};
