#include "buzzer.h"

Buzzer::Buzzer(uint8_t pin)
    : pin_(pin), active_(false), offAtMs_(0) {}

void Buzzer::begin() {
    pinMode(pin_, OUTPUT);
    off();
}

void Buzzer::on() {
    digitalWrite(pin_, HIGH);
    active_ = true;
    offAtMs_ = 0;
}

void Buzzer::off() {
    digitalWrite(pin_, LOW);
    active_ = false;
    offAtMs_ = 0;
}

void Buzzer::beep(uint16_t durationMs) {
    digitalWrite(pin_, HIGH);
    active_ = true;
    offAtMs_ = millis() + durationMs;
}

bool Buzzer::isActive() const {
    return active_;
}
