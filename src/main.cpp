#include <Arduino.h>
#include "pin.h"
#include "config.h"
#include "display.h"
#include "button.h"
#include "rtc.h"
#include "alarm.h"

ClockDisplay clockDisplay(Pins::DISPLAY_CLK, Pins::DISPLAY_DIO);
Button modeButton(Pins::BUTTON_SET);
Button upButton(Pins::BUTTON_UP);
Button downButton(Pins::BUTTON_DOWN);
RTCManager rtc;
AlarmManager alarm;

namespace {
constexpr unsigned long COLON_BLINK_INTERVAL_MS = 1000;
constexpr unsigned long EDIT_BLINK_INTERVAL_MS = 500;
constexpr unsigned long RTC_REFRESH_INTERVAL_MS = 200;
constexpr unsigned long RTC_STATUS_INTERVAL_MS = 5000;
constexpr uint32_t MODE_LONG_PRESS_MS = 3000;

uint8_t currentHour = 12;
uint8_t currentMinute = 34;
uint8_t alarmHour = Config::DEFAULT_ALARM_HOUR;
uint8_t alarmMinute = Config::DEFAULT_ALARM_MINUTE;

enum class EditMode {
    Normal,
    EditHours,
    EditMinutes,
    AlarmHours,
    AlarmMinutes
};

EditMode editMode = EditMode::Normal;

unsigned long lastColonToggle = 0;
unsigned long lastEditBlink = 0;
unsigned long lastRtcRefresh = 0;
unsigned long lastRtcStatus = 0;
bool colonVisible = true;
bool editFieldVisible = true;

bool isTimeEditing() {
    return editMode == EditMode::EditHours || editMode == EditMode::EditMinutes;
}

bool isAlarmEditing() {
    return editMode == EditMode::AlarmHours || editMode == EditMode::AlarmMinutes;
}

void refreshDisplay() {
    if (editMode == EditMode::Normal) {
        clockDisplay.showTime(currentHour, currentMinute, colonVisible);
        return;
    }

    if (isAlarmEditing()) {
        clockDisplay.showEditingTime(
            alarmHour,
            alarmMinute,
            editMode == EditMode::AlarmHours && !editFieldVisible,
            editMode == EditMode::AlarmMinutes && !editFieldVisible,
            colonVisible
        );
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

void changeHour(uint8_t &hour, int8_t amount) {
    int value = static_cast<int>(hour) + amount;
    if (value < 0) {
        value = 23;
    } else if (value > 23) {
        value = 0;
    }
    hour = static_cast<uint8_t>(value);
}

void changeMinute(uint8_t &minute, int8_t amount) {
    int value = static_cast<int>(minute) + amount;
    if (value < 0) {
        value = 59;
    } else if (value > 59) {
        value = 0;
    }
    minute = static_cast<uint8_t>(value);
}

void enterEditHours() {
    readRtcTime();
    editMode = EditMode::EditHours;
    editFieldVisible = true;
    lastEditBlink = millis();
    refreshDisplay();
    Serial.println("Edit mode: CLOCK HOURS");
}

void enterEditMinutes() {
    editMode = EditMode::EditMinutes;
    editFieldVisible = true;
    lastEditBlink = millis();
    refreshDisplay();
    Serial.println("Edit mode: CLOCK MINUTES");
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
    Serial.println("Clock setting complete - HOME");
}

void enterAlarmHours() {
    editMode = EditMode::AlarmHours;
    editFieldVisible = true;
    lastEditBlink = millis();
    refreshDisplay();
    Serial.print("Alarm setup: HOURS | Current alarm: ");
    if (alarmHour < 10) Serial.print('0');
    Serial.print(alarmHour);
    Serial.print(':');
    if (alarmMinute < 10) Serial.print('0');
    Serial.println(alarmMinute);
}

void enterAlarmMinutes() {
    editMode = EditMode::AlarmMinutes;
    editFieldVisible = true;
    lastEditBlink = millis();
    refreshDisplay();
    Serial.println("Alarm setup: MINUTES");
}

void saveAlarmAndGoHome() {
    alarm.setTime(alarmHour, alarmMinute);
    alarm.enable();

    editMode = EditMode::Normal;
    editFieldVisible = true;
    readRtcTime();
    refreshDisplay();

    Serial.print("ALARM SET: ");
    if (alarmHour < 10) Serial.print('0');
    Serial.print(alarmHour);
    Serial.print(':');
    if (alarmMinute < 10) Serial.print('0');
    Serial.println(alarmMinute);
    Serial.println("Alarm enabled - HOME");
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
    alarm.begin(alarmHour, alarmMinute);

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

    Serial.println("MODE short press: clock setting");
    Serial.println("MODE long press (3 seconds): alarm setting");

    refreshDisplay();
    lastRtcRefresh = millis();
    lastRtcStatus = millis();
}

void loop() {
    const unsigned long now = millis();

    // MODE supports two actions from HOME:
    // short press -> clock setting, 3-second hold -> alarm setting.
    const Button::Event modeEvent = modeButton.event(MODE_LONG_PRESS_MS);

    if (modeEvent == Button::Event::LongPress) {
        if (editMode == EditMode::Normal) {
            enterAlarmHours();
        }
    } else if (modeEvent == Button::Event::ShortPress) {
        if (editMode == EditMode::Normal) {
            enterEditHours();
        } else if (editMode == EditMode::EditHours) {
            enterEditMinutes();
        } else if (editMode == EditMode::EditMinutes) {
            exitEditMode();
        } else if (editMode == EditMode::AlarmHours) {
            enterAlarmMinutes();
        } else if (editMode == EditMode::AlarmMinutes) {
            saveAlarmAndGoHome();
        }
    }

    // UP/DOWN work while editing either the clock or the alarm.
    if (editMode == EditMode::EditHours) {
        if (upButton.repeatPressed()) {
            changeHour(currentHour, 1);
        }
        if (downButton.repeatPressed()) {
            changeHour(currentHour, -1);
        }
    } else if (editMode == EditMode::EditMinutes) {
        if (upButton.repeatPressed()) {
            changeMinute(currentMinute, 1);
        }
        if (downButton.repeatPressed()) {
            changeMinute(currentMinute, -1);
        }
    } else if (editMode == EditMode::AlarmHours) {
        if (upButton.repeatPressed()) {
            changeHour(alarmHour, 1);
        }
        if (downButton.repeatPressed()) {
            changeHour(alarmHour, -1);
        }
    } else if (editMode == EditMode::AlarmMinutes) {
        if (upButton.repeatPressed()) {
            changeMinute(alarmMinute, 1);
        }
        if (downButton.repeatPressed()) {
            changeMinute(alarmMinute, -1);
        }
    } else {
        upButton.repeatPressed();
        downButton.repeatPressed();
    }

    // In normal mode the DS1307 remains the time source.
    if (editMode == EditMode::Normal && now - lastRtcRefresh >= RTC_REFRESH_INTERVAL_MS) {
        lastRtcRefresh = now;
        readRtcTime();
    }

    if (now - lastColonToggle >= COLON_BLINK_INTERVAL_MS) {
        lastColonToggle = now;
        colonVisible = !colonVisible;
    }

    if (editMode != EditMode::Normal && now - lastEditBlink >= EDIT_BLINK_INTERVAL_MS) {
        lastEditBlink = now;
        editFieldVisible = !editFieldVisible;
    }

    if (now - lastRtcStatus >= RTC_STATUS_INTERVAL_MS) {
        lastRtcStatus = now;
        rtc.printStatus(Serial);
    }

    refreshDisplay();
}
