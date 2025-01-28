#include "poller.h"
#include "uart.h"
#include "timer.h"
#include "timer_cfg.h"
#include "ADC.h"
#include "ADC_Config.h"
#include "Uart.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Logic.h"
#include "OS_Config.h"

uint16 static pollCounter =0;
uint16 static pollTimeCounter=0;

float cap_percentage = 0;


void poll(void) {
	UART_Init(9600);
	updatePollCounter();
	if ((pollCounter == pollTimeCounter)) {
		pollCounter=0;

		if (1)
		{

						// Variables

				uint16_t adc_value = 0;  // Raw ADC value
				uint16_t adc_OC_Check_value = 0;  // ADC value to check for overcurrent
				uint16_t adc_connection_check = 0;  // ADC value to check for connected load

				float adc_voltage = 0;


//
//				 Read the ADC value from channel 0 (ADC0)
				adc_value = ADC_READ(0);
//				 Convert ADC value to voltage
				adc_voltage = ADC_GetVoltage(adc_value);
				cap_percentage = calculateChargePercentage(adc_voltage);


				adc_connection_check = ADC_READ(VEHICLE_CONNECTION_CHECK_PIN);

				adc_OC_Check_value = ADC_READ(VEHICLE_OVERCURRENT_CHECK_PIN);

				if ((adc_OC_Check_value > OVERCURRENT_THRESHOLD) || (adc_connection_check < CONNECTION_THRESHOLD)){
					xTaskNotifyGive(VehicleCheckTaskHandle);
				}
				// adc_OC_Check = ADC_READ(1);
				// if (adc_OC_Check > OVERCURRENT_THRESHOLD) {
				// 	UART_Print("Overcurrent!\r\n");
				// 	xTaskNotifyGive(Overcurrent_TaskHandle);
				// }
				
				// adc_connection_check = ADC_READ(2);
				// if (adc_connection_check < CONNECTION_THRESHOLD) {
				// 	UART_Print("CONNECTED!\r\n");
				// } else if (adc_connection_check > CONNECTION_THRESHOLD) {
				// 	UART_Print("NOT CONNECTED!\r\n");

				// 	xTaskNotifyGive(SuddenDisconnect_TaskHandle);

				// }

				// Print results to virtual terminal
				UART_Print("Charge Percentage: ");
				UART_PrintFloat(cap_percentage);
				UART_Print("%\r\n");


		}
	}
}

void updatePollCounter(void) {
	pollCounter += 1; 
}

void poller_Init(void) {
	Timer_vidInitTimer1();
	Timer_vidSetCallBackTimer1(poll);
}

void Poller__StartPolling(void) {
	Timer_vidStartTimer1();
}

void Poller__StopPolling(void) {
	Timer_vidStopTimer1();
}


void poller_SetPollTime(uint32 time) {

	pollTimeCounter = time / time_ms;

}
