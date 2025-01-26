#include "FreeRTOS.h"
#include "task.h"
#include "LCD.h"
#include "ADC_Config.h"
#include "ADC.h"
#include "Os.h"
#include "Gpio.h"
#include "Gpio_Config.h"
#include "Uart.h"
#include "poller.h"
#include "Logic.h"
#include <util/delay.h>


int main(void) {

	// Initialize buttons for interrupts
		Button_Init();
		LCD_Init();

		ADC_INIT(128, AREF);

		poller_Init();
		
		Poller__StartPolling();
		poller_SetPollTime(50);
		
		

		Os_Init();


	while(1){
	}

		return 0;
	
}
