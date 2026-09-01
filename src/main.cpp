#include <Arduino.h>
#include "pin.h"
#include "display.h"
#include "button.h"
#include "rtc.h"

ClockDisplay clockDisplay(Pins::DISPLAY_CLK, Pins::DISPLAY_DIO);
Button modeButton(Pins::BUTTON_SET);
Button upButton(Pins::BUTTON_UP);
Button downButton(Pins::BUTTON_DOWN);
RTCManager rtc;

namespace {
constexpr unsigned long COLON_BLINK_INTERVAL_MS = 1000;
constexpr unsigned long EDIT_BLINK_INTERVAL_MS = 500;
constexpr unsigned long RTC_REFRESH_INTERVAL_MS = 200;
constexpr unsigned long RTC_STATUS_INTERVAL_MS = 5000;

uint8_t currentHour = 12;
uint8_t currentMinute = 34;

enum class EditMode {
    Normal,
    EditHours,
    EditMinutes
};

EditMode editMode = EditMode::Normal;

unsigned long lastColonToggle = 0;
unsigned long lastEditBlink = 0;
unsigned long lastRtcRefresh = 0;
unsigned long lastRtcStatus = 0;
bool colonVisible = true;
bool editFieldVisible = true;

void refreshDisplay() {
    if (editMode == EditMode::Normal) {
        clockDisplay.showTime(currentHour, currentMinute, colonVisible);
        return;
    }

    clockDisplay.showEditingTime(
        currentHour,
        currentMinute,
        editMode == EditMode::EditHours && !editFieldVisible,
        editMode == EditMode::EditMinutes && !editFieldVisible,
        colonVisible
    );
}

void readRtcTime() {
    uint8_t hour;
    uint8_t minute;

    if (rtc.getTime(hour, minute)) {
        currentHour = hour;
        currentMinute = minute;
    }
}

void changeHour(int8_t amount) {
    int value = static_cast<int>(currentHour) + amount;
    if (value < 0) {
        value = 23;
    } else if (value > 23) {
        value = 0;
    }
    currentHour = static_cast<uint8_t>(value);
}

void changeMinute(int8_t amount) {
    int value = static_cast<int>(currentMinute) + amount;
    if (value < 0) {
        value = 59;
    } else if (value > 59) {
        value = 0;
    }
    currentMinute = static_cast<uint8_t>(value);
}

void enterEditHours() {
    readRtcTime();
    editMode = EditMode::EditHours;
    editFieldVisible = true;
    lastEditBlink = millis();
    refreshDisplay();
    Serial.println("Edit mode: HOURS");
}

void enterEditMinutes() {
    editMode = EditMode::EditMinutes;
    editFieldVisible = true;
    lastEditBlink = millis();
    refreshDisplay();
    Serial.println("Edit mode: MINUTES");
}

void exitEditMode() {
    if (rtc.setTime(currentHour, currentMinute)) {
        Serial.println("RTC time saved.");
    } else {
        Serial.println("RTC time save failed.");
    }

    editMode = EditMode::Normal;
    editFieldVisible = true;
    readRtcTime();
    refreshDisplay();
    Serial.println("Edit mode: SAVED");
}
}

void setup() {
    Serial.begin(115200);
    delay(100);

    modeButton.begin();
    upButton.begin();
    downButton.begin();
    pinMode(Pins::BUZZER, OUTPUT);
    digitalWrite(Pins::BUZZER, LOW);

    clockDisplay.begin();

    Serial.println();
    Serial.println("ESP8266 Alarm Clock - DS1307 RTC clock");
    Serial.println("Initializing DS1307...");

    if (rtc.begin()) {
        rtc.printStatus(Serial);
        readRtcTime();
    } else {
        Serial.println("RTC communication FAILED.");
        Serial.println("Check DS1307 VCC, GND, SDA and SCL wiring.");
    }

    refreshDisplay();
    lastRtcRefresh = millis();
    lastRtcStatus = millis();
}

void loop() {
    const unsigned long now = millis();

    // MODE advances through normal -> hours -> minutes -> normal.
    if (modeButton.pressed()) {
        if (editMode == EditMode::Normal) {
            enterEditHours();
        } else if (editMode == EditMode::EditHours) {
            enterEditMinutes();
        } else {
            exitEditMode();
        }
    }

    // UP/DOWN work only while editing.
    if (editMode == EditMode::EditHours) {
        if (upButton.repeatPressed()) {
            changeHour(1);
        }
        if (downButton.repeatPressed()) {
            changeHour(-1);
        }
    } else if (editMode == EditMode::EditMinutes) {
        if (upButton.repeatPressed()) {
            changeMinute(1);
        }
        if (downButton.repeatPressed()) {
            changeMinute(-1);
        }
    } else {
        // Keep button state/debounce handling active while not editing.
        upButton.repeatPressed();
        downButton.repeatPressed();
    }

    // In normal mode the DS1307 is the time source. The display therefore
    // changes minute-by-minute and hour-by-hour according to the RTC.
    if (editMode == EditMode::Normal && now - lastRtcRefresh >= RTC_REFRESH_INTERVAL_MS) {
        lastRtcRefresh = now;
        readRtcTime();
    }

    // Blink the colon every second in normal mode and while editing.
    if (now - lastColonToggle >= COLON_BLINK_INTERVAL_MS) {
        lastColonToggle = now;
        colonVisible = !colonVisible;
    }

    // Blink the currently selected hours/minutes field while editing.
    if (editMode != EditMode::Normal && now - lastEditBlink >= EDIT_BLINK_INTERVAL_MS) {
        lastEditBlink = now;
        editFieldVisible = !editFieldVisible;
    }

    // Print the RTC status periodically so communication can be verified
    // in the Serial Monitor without flooding it every loop.
    if (now - lastRtcStatus >= RTC_STATUS_INTERVAL_MS) {
        lastRtcStatus = now;
        rtc.printStatus(Serial);
    }

    refreshDisplay();
}
