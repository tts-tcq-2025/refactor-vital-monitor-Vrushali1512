#include "monitor.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

// Implementation of the pure function
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

// Implementation of the I/O function
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

// Implementation of the main function
int vitalsOk(float temperature, float pulseRate, float spo2) {
    int critical_code = isCritical(temperature, pulseRate, spo2);
    
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
    
    return 1;
}
