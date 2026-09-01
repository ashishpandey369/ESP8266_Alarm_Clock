#include "display.h"

ClockDisplay::ClockDisplay(uint8_t clkPin, uint8_t dioPin)
    : display_(clkPin, dioPin) {}

void ClockDisplay::begin() {
    display_.setBrightness(7, true);
    display_.clear();
}

void ClockDisplay::showTime(uint8_t hour, uint8_t minute, bool colon) {
    const uint16_t value = static_cast<uint16_t>(hour) * 100U + minute;
    display_.showNumberDecEx(value, colon ? 0b01000000 : 0, true);
}

void ClockDisplay::showNumber(int value) {
    display_.showNumberDec(value, true);
}

void ClockDisplay::clear() {
    display_.clear();
}

void ClockDisplay::setBrightness(uint8_t level) {
    if (level > 7) {
        level = 7;
    }
    display_.setBrightness(level, true);
}
