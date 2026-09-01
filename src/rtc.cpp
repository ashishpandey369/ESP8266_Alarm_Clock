#include "rtc.h"

// Implementation will be added after the exact RTC module is confirmed.
// Expected module: DS3231.
//
bool RTCManager::begin() {
    return false;
}

bool RTCManager::isRunning() const {
    return false;
}

bool RTCManager::hasValidTime() const {
    return false;
}

void RTCManager::printStatus(Stream &serial) const {
    serial.println(F("RTC manager not initialized yet."));
}
