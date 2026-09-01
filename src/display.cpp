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

void ClockDisplay::showEditingTime(uint8_t hour, uint8_t minute, bool blinkHours, bool blinkMinutes, bool colon) {
    uint8_t segments[4];
    const uint8_t hourTens = hour / 10;
    const uint8_t hourOnes = hour % 10;
    const uint8_t minuteTens = minute / 10;
    const uint8_t minuteOnes = minute % 10;

    segments[0] = blinkHours ? 0x00 : display_.encodeDigit(hourTens);
    segments[1] = blinkHours ? 0x00 : display_.encodeDigit(hourOnes);
    segments[2] = blinkMinutes ? 0x00 : display_.encodeDigit(minuteTens);
    segments[3] = blinkMinutes ? 0x00 : display_.encodeDigit(minuteOnes);

    // The TM1637 4-digit module uses the center-colon indication through
    // the second digit. Do not set it while the hours field is blank, or
    // it appears as a stray center segment on the blank second digit.
    if (colon && !blinkHours) {
        segments[1] |= 0b01000000;
    }

    display_.setSegments(segments);
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
