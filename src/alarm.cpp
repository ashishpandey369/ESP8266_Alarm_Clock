#include "alarm.h"

AlarmManager::AlarmManager()
    : hour_(0), minute_(0), enabled_(false) {}

void AlarmManager::begin(uint8_t defaultHour, uint8_t defaultMinute) {
    if (defaultHour > 23) defaultHour = 0;
    if (defaultMinute > 59) defaultMinute = 0;

    hour_ = defaultHour;
    minute_ = defaultMinute;
    enabled_ = false;
}

void AlarmManager::setTime(uint8_t hour, uint8_t minute) {
    if (hour > 23 || minute > 59) {
        return;
    }

    hour_ = hour;
    minute_ = minute;
}

uint8_t AlarmManager::getHour() const {
    return hour_;
}

uint8_t AlarmManager::getMinute() const {
    return minute_;
}

void AlarmManager::enable() {
    enabled_ = true;
}

void AlarmManager::disable() {
    enabled_ = false;
}

bool AlarmManager::isEnabled() const {
    return enabled_;
}

bool AlarmManager::matches(uint8_t hour, uint8_t minute) const {
    return enabled_ && hour == hour_ && minute == minute_;
}
