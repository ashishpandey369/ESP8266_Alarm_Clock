#include "button.h"

namespace {
constexpr uint32_t DEBOUNCE_MS = 35;
}

Button::Button(uint8_t pin)
    : pin_(pin),
      lastState_(HIGH),
      stableState_(HIGH),
      lastChangeMs_(0),
      pressStartMs_(0),
      lastRepeatMs_(0) {}

void Button::begin() {
    pinMode(pin_, INPUT_PULLUP);
    lastState_ = digitalRead(pin_);
    stableState_ = lastState_;
    lastChangeMs_ = millis();
    pressStartMs_ = 0;
    lastRepeatMs_ = 0;
}

bool Button::pressed() {
    const bool currentState = digitalRead(pin_);

    if (currentState != lastState_) {
        lastState_ = currentState;
        lastChangeMs_ = millis();
    }

    if ((millis() - lastChangeMs_) >= DEBOUNCE_MS && currentState != stableState_) {
        stableState_ = currentState;
        if (stableState_ == LOW) {
            pressStartMs_ = millis();
            lastRepeatMs_ = pressStartMs_;
            return true;
        }
    }

    return false;
}

bool Button::repeatPressed(uint32_t initialDelayMs, uint32_t repeatIntervalMs) {
    const bool currentState = digitalRead(pin_);

    if (currentState != lastState_) {
        lastState_ = currentState;
        lastChangeMs_ = millis();
    }

    const uint32_t now = millis();

    if ((now - lastChangeMs_) >= DEBOUNCE_MS && currentState != stableState_) {
        stableState_ = currentState;
        if (stableState_ == LOW) {
            pressStartMs_ = now;
            lastRepeatMs_ = now;
            return true;
        }
    }

    if (stableState_ == LOW &&
        (now - pressStartMs_) >= initialDelayMs &&
        (now - lastRepeatMs_) >= repeatIntervalMs) {
        lastRepeatMs_ = now;
        return true;
    }

    return false;
}
