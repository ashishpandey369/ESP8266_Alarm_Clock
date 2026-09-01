#include "rtc.h"
#include "pin.h"
#include <Wire.h>

bool RTCManager::begin() {
    Wire.begin(Pins::RTC_SDA, Pins::RTC_SCL);

    initialized_ = rtc_.begin(&Wire);
    if (!initialized_) {
        return false;
    }

    // If the DS1307 oscillator is stopped, initialize it once from the
    // firmware build time. A running RTC is never overwritten at boot.
    if (!rtc_.isrunning()) {
        rtc_.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    return true;
}

bool RTCManager::isRunning() {
    return initialized_ && rtc_.isrunning();
}

bool RTCManager::hasValidTime() {
    if (!initialized_) {
        return false;
    }

    const DateTime now = rtc_.now();
    return now.isValid() && now.year() >= 2000 && now.year() <= 2099;
}

bool RTCManager::getTime(uint8_t &hour, uint8_t &minute) {
    if (!initialized_) {
        return false;
    }

    const DateTime now = rtc_.now();
    if (!now.isValid()) {
        return false;
    }

    hour = now.hour();
    minute = now.minute();
    return true;
}

bool RTCManager::setTime(uint8_t hour, uint8_t minute) {
    if (!initialized_ || hour > 23 || minute > 59) {
        return false;
    }

    const DateTime current = rtc_.now();
    rtc_.adjust(DateTime(
        current.year(),
        current.month(),
        current.day(),
        hour,
        minute,
        0
    ));

    return true;
}

void RTCManager::printStatus(Stream &serial) {
    if (!initialized_) {
        serial.println(F("RTC: NOT FOUND (DS1307 did not respond at 0x68)"));
        return;
    }

    const DateTime now = rtc_.now();

    serial.print(F("RTC: DS1307 OK | Running: "));
    serial.print(rtc_.isrunning() ? F("YES") : F("NO"));
    serial.print(F(" | Time: "));
    if (now.hour() < 10) serial.print('0');
    serial.print(now.hour());
    serial.print(':');
    if (now.minute() < 10) serial.print('0');
    serial.print(now.minute());
    serial.print(':');
    if (now.second() < 10) serial.print('0');
    serial.print(now.second());
    serial.print(F(" | Date: "));
    serial.print(now.year());
    serial.print('-');
    if (now.month() < 10) serial.print('0');
    serial.print(now.month());
    serial.print('-');
    if (now.day() < 10) serial.print('0');
    serial.println(now.day());
}
