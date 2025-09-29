#include "./monitor.h"
#include <stdio.h>
#include <unistd.h>

#define TEMP_TOLERANCE (0.015 * 102)
#define PULSE_TOLERANCE (0.015 * 100)
#define SPO2_TOLERANCE (0.015 * 100)

typedef struct {
    int code;
    int (*condition)(float temperature, float pulseRate, float spo2);
} ConditionCheck;

static int tempHighCritical(float t, float p, float s) { return t > 102; }
static int tempLowCritical(float t, float p, float s) { return t < 95; }
static int pulseLowCritical(float t, float p, float s) { return p < 60; }
static int pulseHighCritical(float t, float p, float s) { return p > 100; }
static int spo2LowCritical(float t, float p, float s) { return s < 90; }

int isCritical(float temperature, float pulseRate, float spo2) {
    static const ConditionCheck criticals[] = {
        {1, tempHighCritical},
        {1, tempLowCritical},
        {2, pulseLowCritical},
        {2, pulseHighCritical},
        {3, spo2LowCritical},
    };
    for (int i = 0; i < sizeof(criticals)/sizeof(criticals[0]); i++) {
        if (criticals[i].condition(temperature, pulseRate, spo2)) {
            return criticals[i].code;
        }
    }
    return 0;
}

static int tempHighWarning(float t, float p, float s) { return t > 102 - TEMP_TOLERANCE; }
static int tempLowWarning(float t, float p, float s) { return t < 95 + TEMP_TOLERANCE; }
static int pulseHighWarning(float t, float p, float s) { return p > 100 - PULSE_TOLERANCE; }
static int pulseLowWarning(float t, float p, float s) { return p < 60 + PULSE_TOLERANCE; }
static int spo2LowWarning(float t, float p, float s) { return s < 90 + SPO2_TOLERANCE; }

int isWarning(float temperature, float pulseRate, float spo2) {
    static const ConditionCheck warnings[] = {
        {1, tempHighWarning},
        {2, tempLowWarning},
        {3, pulseHighWarning},
        {4, pulseLowWarning},
        {5, spo2LowWarning},
    };
    for (int i = 0; i < sizeof(warnings)/sizeof(warnings[0]); i++) {
        if (warnings[i].condition(temperature, pulseRate, spo2)) {
            return warnings[i].code;
        }
    }
    return 0;
}

void handleAlert(const char* message) {
    printf("%s\n", message);
    for (int i = 0; i < 6; i++) {
        printf("\r* ");
        fflush(stdout);
        sleep(1);
        printf("\r *");
        fflush(stdout);
        sleep(1);
    }
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    int critical_code = isCritical(temperature, pulseRate, spo2);
    if (critical_code != 0) {
        static const char* criticalMessages[] = {
            "",  // 0 unused
            "Temperature is critical!",
            "Pulse Rate is out of range!",
            "Oxygen Saturation out of range!"
        };
        handleAlert(criticalMessages[critical_code]);
        return 0;
    }

    int warning_code = isWarning(temperature, pulseRate, spo2);
    if (warning_code != 0) {
        static const char* warningMessages[] = {
            "",  // 0 unused
            "Warning: Approaching hyperthermia",
            "Warning: Approaching hypothermia",
            "Warning: Approaching high pulse rate",
            "Warning: Approaching low pulse rate",
            "Warning: Approaching low oxygen saturation"
        };
        printf("%s\n", warningMessages[warning_code]);
    }

    return 1;
}
