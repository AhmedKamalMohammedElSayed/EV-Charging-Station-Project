/*
 * Logic.h
 *
 * Created: 24/01/2025 04:52:07 م
 *  Author: Ahmed
 */ 


#include "avr/io.h"

// Define Button Pins
#define START_BUTTON_PIN PD2
#define STOP_BUTTON_PIN PD3

// Define constants for the battery (capacitor)
#define MAX_VOLTAGE 4.25
#define MIN_VOLTAGE 0.63


void Charge_Logic(void);
void Stop_Charge_Logic(void);
void Button_Init(void);
float calculateChargePercentage(float voltage);
float calculateTimeRemaining(float currentVoltage, float targetVoltage, float chargingCurrent, float capacitorValue);
