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




// Display Refresh Task (100ms)
void TaskDisplayRefresh(void *pvParameters) {
	TickType_t xLastWakeTime; // Stores the last wake time
	const TickType_t xFrequency = pdMS_TO_TICKS(100); // Task frequency: 100ms

	// Initialize the xLastWakeTime with the current time
	xLastWakeTime = xTaskGetTickCount();
//	Charge_Logic();

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
	TickType_t xLastWakeTime; // Stores the last wake time
	const TickType_t xFrequency = pdMS_TO_TICKS(500); // Task frequency: 100ms

	// Initialize the xLastWakeTime with the current time
	xLastWakeTime = xTaskGetTickCount();
    // Configure PC0 as output
    DDRC |= (1 << PC0);

    while (1) {
        // Simulate charging logic (set PC0 HIGH)
        PORTC |= (1 << PC0);

        // Delay for 500ms
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

        // Signal TaskStopSession by giving the semaphore
        xSemaphoreGive(chargingSemaphore);

        // Suspend this task after signaling
        vTaskSuspend(NULL);
    }
}


// Stop Session Task (100ms)
void TaskStopSession(void *pvParameters) {
	TickType_t xLastWakeTime; // Stores the last wake time
	const TickType_t xFrequency = pdMS_TO_TICKS(2000); // Task frequency: 100ms

	// Initialize the xLastWakeTime with the current time
	xLastWakeTime = xTaskGetTickCount();
    while (1) {
        // Wait for the semaphore from TaskChargingStart
        if (xSemaphoreTake(chargingSemaphore, portMAX_DELAY) == pdTRUE) {
            // Execute stop session logic
            while (1) {
                Stop_Charge_Logic();
        		vTaskDelayUntil(&xLastWakeTime, xFrequency);
            }
        }
    }
}


void Os_Init(void) {
	 // Create the event group
    chargingSemaphore = xSemaphoreCreateBinary();

	// Task creation
	xTaskCreate(TaskChargingStart, "ChargingStart", 128, NULL, 3, &TaskChargingStartHandle);

	xTaskCreate(TaskStopSession, "StopSession", 128, NULL, 2, &TaskStopSessionHandle);
	xTaskCreate(TaskDisplayRefresh, "DisplayRefresh", 128, NULL, 1, &TaskDisplayRefreshHandle);
	//xTaskCreate(TaskUserCommand, "UserCommand", 128, NULL, 2, &TaskUserCommandHandle);
	//xTaskCreate(TaskFaultDetection, "FaultDetection", 128, NULL, 4, &TaskFaultDetectionHandle);

	vTaskStartScheduler();

}
