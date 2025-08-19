#include "monitor.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

// Early warning tolerances
#define TEMP_TOLERANCE 0.015 * 102
#define PULSE_TOLERANCE 0.015 * 100
#define SPO2_TOLERANCE 0.015 * 100

int isCritical(float temperature, float pulseRate, float spo2) {
    if (temperature > 102 || temperature < 95) {
        return 1;
    }
    if (pulseRate < 60 || pulseRate > 100) {
        return 2;
    }
    if (spo2 < 90) {
        return 3;
    }
    return 0;
}

int isWarning(float temperature, float pulseRate, float spo2) {
    if (temperature > 102 - TEMP_TOLERANCE) {
        return 1; // Approaching hyperthermia
    }
    if (temperature < 95 + TEMP_TOLERANCE) {
        return 2; // Approaching hypothermia
    }
    if (pulseRate > 100 - PULSE_TOLERANCE) {
        return 3; // Approaching high pulse rate
    }
    if (pulseRate < 60 + PULSE_TOLERANCE) {
        return 4; // Approaching low pulse rate
    }
    if (spo2 < 90 + SPO2_TOLERANCE) {
        return 5; // Approaching low oxygen saturation
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
    int warning_code = isWarning(temperature, pulseRate, spo2);
    
    if (critical_code == 1) {
        handleAlert("Temperature is critical!");
        return 0;
    } else if (critical_code == 2) {
        handleAlert("Pulse Rate is out of range!");
        return 0;
    } else if (critical_code == 3) {
        handleAlert("Oxygen Saturation out of range!");
        return 0;
    }

    if (warning_code == 1) {
        printf("Warning: Approaching hyperthermia\n");
    } else if (warning_code == 2) {
        printf("Warning: Approaching hypothermia\n");
    } else if (warning_code == 3) {
        printf("Warning: Approaching high pulse rate\n");
    } else if (warning_code == 4) {
        printf("Warning: Approaching low pulse rate\n");
    } else if (warning_code == 5) {
        printf("Warning: Approaching low oxygen saturation\n");
    }
    
    return 1;
}
