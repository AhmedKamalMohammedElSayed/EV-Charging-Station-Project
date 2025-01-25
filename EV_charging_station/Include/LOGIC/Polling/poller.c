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

uint16 static pollCounter =0;
uint16 static pollTimeCounter=0;



void poll(void) {
	UART_Init(9600);
	updatePollCounter();
	if ((pollCounter == pollTimeCounter)) {
		pollCounter=0;
		//DeviceStateType tc72_state = TC72_CheckSensorState();

		if (1)
		{
				//DeviceDataType raw_temperature = TC72_ReadTemperature();
				//DeviceDataType current_temp = raw_temperature / 4;
				//DisplayTemperature(raw_temperature);
				//float32 dutyCycle = PID_Controller(30.0, current_temp);
				//SetPWMDutyCycle(dutyCycle);

						// Variables

				uint16_t adc_value = 0;  // Raw ADC value
//
//				 Read the ADC value from channel 0 (ADC0)
				adc_value = ADC_READ(0);
//				 Convert ADC value to voltage
//
				char buffer[32];         // Buffer for sending text over UART
//
				snprintf(buffer, sizeof(buffer), "ADC: %d\n", adc_value );
				UART_SendString(buffer);
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
