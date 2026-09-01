#include "alarm.h"
#include <EEPROM.h>

namespace {
constexpr int EEPROM_SIZE = 16;
constexpr int EEPROM_MAGIC_ADDR = 0;
constexpr int EEPROM_HOUR_ADDR = 1;
constexpr int EEPROM_MINUTE_ADDR = 2;
constexpr int EEPROM_ENABLED_ADDR = 3;
constexpr uint8_t EEPROM_MAGIC = 0xA7;
}

AlarmManager::AlarmManager()
    : hour_(0), minute_(0), enabled_(false) {}

void AlarmManager::begin(uint8_t defaultHour, uint8_t defaultMinute) {
    if (defaultHour > 23) defaultHour = 0;
    if (defaultMinute > 59) defaultMinute = 0;

    EEPROM.begin(EEPROM_SIZE);

    const uint8_t magic = EEPROM.read(EEPROM_MAGIC_ADDR);
    const uint8_t storedHour = EEPROM.read(EEPROM_HOUR_ADDR);
    const uint8_t storedMinute = EEPROM.read(EEPROM_MINUTE_ADDR);
    const uint8_t storedEnabled = EEPROM.read(EEPROM_ENABLED_ADDR);

    if (magic == EEPROM_MAGIC && storedHour <= 23 && storedMinute <= 59 && storedEnabled <= 1) {
        hour_ = storedHour;
        minute_ = storedMinute;
        enabled_ = storedEnabled == 1;
        return;
    }

    hour_ = defaultHour;
    minute_ = defaultMinute;
    enabled_ = false;
    save();
}

void AlarmManager::setTime(uint8_t hour, uint8_t minute) {
    if (hour > 23 || minute > 59) {
        return;
    }

    hour_ = hour;
    minute_ = minute;
    save();
}

uint8_t AlarmManager::getHour() const {
    return hour_;
}

uint8_t AlarmManager::getMinute() const {
    return minute_;
}

void AlarmManager::enable() {
    enabled_ = true;
    save();
}

void AlarmManager::disable() {
    enabled_ = false;
    save();
}

bool AlarmManager::isEnabled() const {
    return enabled_;
}

bool AlarmManager::matches(uint8_t hour, uint8_t minute) const {
    return enabled_ && hour == hour_ && minute == minute_;
}

void AlarmManager::save() const {
    EEPROM.write(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);
    EEPROM.write(EEPROM_HOUR_ADDR, hour_);
    EEPROM.write(EEPROM_MINUTE_ADDR, minute_);
    EEPROM.write(EEPROM_ENABLED_ADDR, enabled_ ? 1 : 0);
    EEPROM.commit();
}
