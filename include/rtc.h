#pragma once

#include <Arduino.h>
#include <RTClib.h>

class RTCManager {
public:
    bool begin();
    bool isRunning();
    bool hasValidTime();
    bool getTime(uint8_t &hour, uint8_t &minute);
    bool setTime(uint8_t hour, uint8_t minute);
    void printStatus(Stream &serial);

private:
    RTC_DS1307 rtc_;
    bool initialized_ = false;
};
