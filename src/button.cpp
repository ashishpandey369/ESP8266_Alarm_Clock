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
      lastRepeatMs_(0),
      longPressReported_(false) {}

void Button::begin() {
    pinMode(pin_, INPUT_PULLUP);
    lastState_ = digitalRead(pin_);
    stableState_ = lastState_;
    lastChangeMs_ = millis();
    pressStartMs_ = 0;
    lastRepeatMs_ = 0;
    longPressReported_ = false;
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
            longPressReported_ = false;
            return true;
        }
    }

    return false;
}

Button::Event Button::event(uint32_t longPressMs) {
    const bool currentState = digitalRead(pin_);
    const uint32_t now = millis();

    if (currentState != lastState_) {
        lastState_ = currentState;
        lastChangeMs_ = now;
    }

    if ((now - lastChangeMs_) < DEBOUNCE_MS) {
        return Event::None;
    }

    if (currentState != stableState_) {
        stableState_ = currentState;

        if (stableState_ == LOW) {
            pressStartMs_ = now;
            longPressReported_ = false;
            return Event::None;
        }

        // A release is a short press only if the long-press event was not
        // already generated while the button was held.
        if (!longPressReported_) {
            return Event::ShortPress;
        }
        return Event::None;
    }

    if (stableState_ == LOW &&
        !longPressReported_ &&
        (now - pressStartMs_) >= longPressMs) {
        longPressReported_ = true;
        return Event::LongPress;
    }

    return Event::None;
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
