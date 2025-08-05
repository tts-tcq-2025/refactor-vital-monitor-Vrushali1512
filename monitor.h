#pragma once
int isCritical(float temperature, float pulseRate, float spo2);
void handleAlert(const char* message);
int vitalsOk(float temperature, float pulseRate, float spo2);

