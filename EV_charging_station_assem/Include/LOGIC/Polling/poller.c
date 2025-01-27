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
				float adc_voltage = 0;
//
//				 Read the ADC value from channel 0 (ADC0)
				adc_value = ADC_READ(0);
//				 Convert ADC value to voltage
				adc_voltage = ADC_GetVoltage(adc_value);
				cap_percentage = calculateChargePercentage(adc_voltage);
//
				// Print results to virtual terminal
				UART_Print("Charge Percentage: ");
				UART_PrintFloat(cap_percentage);
				UART_Print("%\r\n");

//				UART_Print("Time Remaining for Charging: ");
//				UART_PrintFloat(timeRemaining);
//				UART_Print(" seconds\r\n");
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
