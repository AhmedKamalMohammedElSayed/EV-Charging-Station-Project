/*
 * Os.c
 *
 * Created: 24/01/2025 07:40:02 م
 *  Author: Ahmed
 */ 
#include "Os.h"
#include "Lcd.h"
#include "ADC_Config.h"
#include "ADC.h"
#include "Logic.h"
#include <util/delay.h>
#include "avr/interrupt.h"




// Display Refresh Task (100ms)
void TaskDisplayRefresh(void *pvParameters) {
	TickType_t xLastWakeTime; // Stores the last wake time
	const TickType_t xFrequency = pdMS_TO_TICKS(100); // Task frequency: 100ms

	// Initialize the xLastWakeTime with the current time
	xLastWakeTime = xTaskGetTickCount();
//

	while (1) {

		// Clear the LCD screen for the next refresh
		//LCD_Init();

		LCD_Clear(); // Clear the LCD screen

		// Display the current charging percentage
		LCD_String("Charge: ");
		LCD_Float(4.4); // Display the percentage
		LCD_String("%");

		// Go to the second line and display the capacitor voltage
		LCD_Command(0xC0); // Go to second line
		LCD_String("Voltage: ");
		LCD_Float(3.3); // Display the capacitor voltage

		//// If charging is complete, display "Full Charge"
		//if (chargePercentage >= 100.0) {
			//LCD_Clear(); // Clear the LCD screen
			//LCD_String("Full Charge");
		//}

		// Delay until the next 100ms interval
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

// User Command Response Task (100ms)
void TaskUserCommand(void *pvParameters) {
	while (1) {
	}
}

// Fault Detection Task (150ms)
void TaskFaultDetection(void *pvParameters) {
	while (1) {
	}
}
void TaskChargingStart(void *pvParameters) {
    uint8_t event;
    for (;;) {
        if (xQueueReceive(buttonQueue, &event, portMAX_DELAY)) {
            if (event == START_BUTTON_EVENT) {
                Charge_Logic();
//                vTaskDelay(pdMS_TO_TICKS(500)); // Simulate stop logic duration
            }
        }
    }
}

void TaskStopSession(void *pvParameters) {
    uint8_t event;
    for (;;) {
        if (xQueueReceive(buttonQueue, &event, portMAX_DELAY)) {
            if (event == STOP_BUTTON_EVENT) {
                Stop_Charge_Logic();
//                vTaskDelay(pdMS_TO_TICKS(200)); // Simulate stop logic duration
            }
        }
    }
}




#define DEBOUNCE_TIME_MS 20  // 20 ms debounce time

ISR(INT0_vect) {

        DDRD |= (1 << PD4);  // Set PC1 as output
	   PORTD |= (1 << PD4); // Turn on alternative indicator

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        uint8_t event = START_BUTTON_EVENT;
        xQueueSendFromISR(buttonQueue, &event, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken) {
            portYIELD();
        }

}



ISR(INT1_vect) { // Stop Button Interrupt

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint8_t event = STOP_BUTTON_EVENT;
		xQueueSendFromISR(buttonQueue, &event, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			portYIELD();
		}

}


void Os_Init(void) {

	// Create Queue
	buttonQueue = xQueueCreate(5, sizeof(uint8_t));
	if (buttonQueue == NULL) {
		// Handle Queue Creation Failure
	}

	// Task creation
	xTaskCreate(TaskChargingStart, "ChargingStart", 128, NULL, 3, &TaskChargingStartHandle);


	xTaskCreate(TaskStopSession, "StopSession", 128, NULL, 3, &TaskStopSessionHandle);
	xTaskCreate(TaskDisplayRefresh, "DisplayRefresh", 128, NULL, 1, &TaskDisplayRefreshHandle);
	//xTaskCreate(TaskUserCommand, "UserCommand", 128, NULL, 2, &TaskUserCommandHandle);
	//xTaskCreate(TaskFaultDetection, "FaultDetection", 128, NULL, 4, &TaskFaultDetectionHandle);

	vTaskStartScheduler();
//	vTaskDelay(pdMS_TO_TICKS(100));

}
