/*
 * Os.c
 *
 * Created: 24/01/2025 07:40:02 م
 * Author: Ahmed
 */ 
#include "Os.h"
#include "Lcd.h"
#include "ADC_Config.h"
#include "ADC.h"
#include "Logic.h"
#include <util/delay.h>
#include "avr/interrupt.h"


// Define system states
typedef enum {
    NONE,
    CHARGING,
    DISCHARGING,
    FAULT_DETECTION
} SystemState;

// Global variable for system state
volatile SystemState globalState = NONE; // Default state


//// Interrupt Service Routine for START Button (INT0)
ISR(INT0_vect) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t event = START_BUTTON_EVENT;
    xQueueSendFromISR(buttonQueue, &event, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken) {
        portYIELD();
    }
}

// Interrupt Service Routine for STOP Button (INT1)
ISR(INT1_vect) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t event = STOP_BUTTON_EVENT;
    xQueueSendFromISR(buttonQueue, &event, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken) {
        portYIELD();
    }
}

// Task to Refresh LCD Display
void TaskDisplayRefresh(void *pvParameters) {
	 TickType_t xLastWakeTime = xTaskGetTickCount(); // Initialize with the current tick count
	const TickType_t xFrequency = pdMS_TO_TICKS(100);


    for(;;) {
    	DDRD |= (1 << PD6);
//		PORTC |= (1 << PD6);

		PIND |= (1<<PD6);


        // Acquire the LCD semaphore for exclusive access
//        if (xSemaphoreTake(lcdSemaphore, portMAX_DELAY) == pdTRUE) {
            LCD_Clear(); // Clear the display
            LCD_String("State:"); // Print label

            // Safely access the global system state
            SystemState localState;
            if (xSemaphoreTake(stateSemaphore, portMAX_DELAY) == pdTRUE) {

            	localState = globalState;
                xSemaphoreGive(stateSemaphore); // Release the state semaphore
            }

            // Update the display based on the current system state
            switch (localState) {
                case NONE:
                    LCD_String("NONE"); // Display NONE state
                    break;
                case CHARGING:
                    LCD_String("Charging"); // Display CHARGING state
                    LCD_Command(0xC0); // Move to the second line
                    LCD_Float(cap_percentage); // Display real-time capacity percentage
                    break;
                case DISCHARGING:
                    LCD_String("Discharging"); // Display DISCHARGING state
                    LCD_Command(0xC0); // Move to the second line
                    LCD_Float(cap_percentage); // Display real-time capacity percentage
                    break;
                case FAULT_DETECTION:
                    LCD_String("Fault"); // Display FAULT state
                    break;
            }

//            xSemaphoreGive(lcdSemaphore); // Release the LCD semaphore
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
//            vTaskDelay(xFrequency);
        }
        // Wait until the next cycle (ensures periodic execution)
    }
//        vTaskDelay(xFrequency);






// Task to Handle Charging Start
void TaskChargingStart(void *pvParameters) {
    uint8_t event;
    for (;;) {
        if (xQueueReceive(buttonQueue, &event, portMAX_DELAY)) {
            if (event == START_BUTTON_EVENT) {
                Charge_Logic();

                if (xSemaphoreTake(stateSemaphore, portMAX_DELAY) == pdTRUE) {
                    globalState = CHARGING;
                    xSemaphoreGive(stateSemaphore);
                }


            }
        }
    }
}

// Task to Handle Stop Session
void TaskStopSession(void *pvParameters) {
    uint8_t event;
    for (;;) {
        if (xQueueReceive(buttonQueue, &event, portMAX_DELAY)) {
            if (event == STOP_BUTTON_EVENT) {
                Stop_Charge_Logic();

                if (xSemaphoreTake(stateSemaphore, portMAX_DELAY) == pdTRUE) {
                    globalState = DISCHARGING;
                    xSemaphoreGive(stateSemaphore);
                }


            }
        }
    }
}

// OS Initialization
void Os_Init(void) {
    // Initialize peripherals
	LCD_Init();

////     Create binary semaphore for LCD access
//    lcdSemaphore = xSemaphoreCreateBinary();
//    if (lcdSemaphore == NULL) {
//        while (1); // Handle semaphore creation failure
//    }
//    xSemaphoreGive(lcdSemaphore);

    // Create binary semaphore for global state
    stateSemaphore = xSemaphoreCreateBinary();
//    if (stateSemaphore == NULL) {
//        while (1); // Handle semaphore creation failure
//    }
    xSemaphoreGive(stateSemaphore);

    // Create the button queue
    buttonQueue = xQueueCreate(5, sizeof(uint8_t));
//    if (buttonQueue == NULL) {
//        while (1); // Handle queue creation failure
//    }

//     Create tasks
    xTaskCreate(TaskDisplayRefresh, "DisplayRefresh", 128, NULL, 4, &taskHandleDisplayRefresh);
    xTaskCreate(TaskChargingStart, "ChargingStart", 128, NULL, 3, NULL);
    xTaskCreate(TaskStopSession, "StopSession", 128, NULL, 3, NULL);
//    xTaskCreate(prd_Buttons, "prd_Buttons", 128, NULL, 4, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}
