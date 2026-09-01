#pragma once

#include <Arduino.h>

class Buzzer {
public:
    explicit Buzzer(uint8_t pin);

    void begin();
    void on();
    void off();
    void beep(uint16_t durationMs);
    void startAlarm();
    void updateAlarm();
    void stopAlarm();
    bool isActive() const;

private:
    uint8_t pin_;
    bool active_;
    uint32_t offAtMs_;
    uint32_t lastToneToggleMs_;
    bool toneOn_;
};
