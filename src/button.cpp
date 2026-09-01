#include "button.h"

namespace {
constexpr uint32_t DEBOUNCE_MS = 35;
}

Button::Button(uint8_t pin)
    : pin_(pin), lastState_(HIGH), stableState_(HIGH), lastChangeMs_(0) {}

void Button::begin() {
    pinMode(pin_, INPUT_PULLUP);
    lastState_ = digitalRead(pin_);
    stableState_ = lastState_;
    lastChangeMs_ = millis();
}

bool Button::pressed() {
    const bool currentState = digitalRead(pin_);

    if (currentState != lastState_) {
        lastState_ = currentState;
        lastChangeMs_ = millis();
    }

    if ((millis() - lastChangeMs_) >= DEBOUNCE_MS && currentState != stableState_) {
        stableState_ = currentState;
        return stableState_ == LOW;
    }

    return false;
}
