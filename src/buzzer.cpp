#include "buzzer.h"

namespace {
constexpr uint16_t ALARM_FREQUENCY_HZ = 2200;
constexpr uint32_t ALARM_TONE_ON_MS = 700;
constexpr uint32_t ALARM_TONE_OFF_MS = 300;
}

Buzzer::Buzzer(uint8_t pin)
    : pin_(pin),
      active_(false),
      offAtMs_(0),
      lastToneToggleMs_(0),
      toneOn_(false) {}

void Buzzer::begin() {
    pinMode(pin_, OUTPUT);
    off();
}

void Buzzer::on() {
    tone(pin_, ALARM_FREQUENCY_HZ);
    active_ = true;
    offAtMs_ = 0;
}

void Buzzer::off() {
    noTone(pin_);
    digitalWrite(pin_, LOW);
    active_ = false;
    offAtMs_ = 0;
    toneOn_ = false;
}

void Buzzer::beep(uint16_t durationMs) {
    tone(pin_, ALARM_FREQUENCY_HZ);
    active_ = true;
    offAtMs_ = millis() + durationMs;
}

void Buzzer::startAlarm() {
    active_ = true;
    toneOn_ = true;
    lastToneToggleMs_ = millis();
    tone(pin_, ALARM_FREQUENCY_HZ);
}

void Buzzer::updateAlarm() {
    if (!active_) {
        return;
    }

    const uint32_t now = millis();
    const uint32_t interval = toneOn_ ? ALARM_TONE_ON_MS : ALARM_TONE_OFF_MS;

    if (now - lastToneToggleMs_ >= interval) {
        lastToneToggleMs_ = now;
        toneOn_ = !toneOn_;

        if (toneOn_) {
            tone(pin_, ALARM_FREQUENCY_HZ);
        } else {
            noTone(pin_);
        }
    }
}

void Buzzer::stopAlarm() {
    off();
}

bool Buzzer::isActive() const {
    return active_;
}
