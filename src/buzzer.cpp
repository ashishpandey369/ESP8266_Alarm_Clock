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

void Buzzer::runDiagnostic(Stream &serial) {
    // Make absolutely sure no previous tone is running.
    off();

    serial.println();
    serial.println(F("========== BUZZER DIAGNOSTIC =========="));
    serial.print(F("Buzzer GPIO: "));
    serial.println(pin_);

    // TEST 1: static HIGH. Useful for an active buzzer or for checking
    // whether the transistor stage switches at all.
    serial.println(F("TEST 1: D0 HIGH for 2 seconds..."));
    digitalWrite(pin_, HIGH);
    delay(2000);
    digitalWrite(pin_, LOW);
    serial.println(F("TEST 1 complete."));

    delay(500);

    // TEST 2: passive piezo tone.
    serial.println(F("TEST 2: 2.2 kHz tone for 3 seconds..."));
    tone(pin_, ALARM_FREQUENCY_HZ);
    delay(3000);
    noTone(pin_);
    digitalWrite(pin_, LOW);
    serial.println(F("TEST 2 complete."));

    serial.println(F("========== DIAGNOSTIC COMPLETE =========="));
    serial.println(F("If TEST 1 or TEST 2 produced sound, the buzzer hardware is responding."));
    serial.println(F("If both were silent, check buzzer/transistor wiring and buzzer type."));
    serial.println();
}
