/*
 * ADC.h
 *
 *  Created on: Jan 23, 2025
 *      Author: medo4
 */

#ifndef INCLUDE_MCAL_ADC_ADC_H_
#define INCLUDE_MCAL_ADC_ADC_H_
#include <stdint.h>


void ADC_INIT(uint8_t prescaler,uint8_t Voltage_Reference);

void ADC_PRESCALER_SET(uint8_t prescaler);
void ADC_voltage_Reference(uint8_t Reference_Voltage);

uint16_t ADC_READ(uint8_t channel);
float ADC_GetVoltage(uint16_t adc_value);  // Convert ADC value to voltage

#endif
