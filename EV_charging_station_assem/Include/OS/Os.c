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
    // TickType_t xLastWakeTime = xTaskGetTickCount(); // Initialize with the current tick count
	// const TickType_t xFrequency = pdMS_TO_TICKS(100);

    static SystemState lastState = NONE; // Remember the last displayed state
    static float lastPercentage = -1.0f; // Remember the last capacity percentage

    for(;;) {
        PIND |= (1<<PD6);
        // If state has changed or needs to be updated, refresh LCD
        SystemState localState;
        float localPercentage = cap_percentage; // Capture percentage at the start
        
        if (xSemaphoreTake(stateSemaphore, portMAX_DELAY) == pdTRUE) {
            localState = globalState;
            xSemaphoreGive(stateSemaphore); // Release the state semaphore
        }

        // Only update the LCD if the state or percentage has changed
        if (localState != lastState || localPercentage != lastPercentage) {
            // Update only the necessary parts of the display
            if (localState != lastState) {
                LCD_Clear(); // Clear only if state has changed
                LCD_String("State:");
                switch (localState) {
                    case NONE:
                        LCD_String("NONE");
                        break;
                    case CHARGING:
                        LCD_String("Charge");
                        LCD_Command(0xC0); // Move to second line
                        LCD_String("Percent: ");
                        LCD_Float(localPercentage);
                        LCD_String("%");
                        break;
                    case DISCHARGING:
                        LCD_String("Discharge");
                        LCD_Command(0xC0); // Move to second line
                        LCD_String("Percent: ");
                        LCD_Float(localPercentage);
                        LCD_String("%");
                        break;
                    case FAULT_DETECTION:
                        LCD_String("Fault");
                        break;
                }
            } else if ((localPercentage != lastPercentage) && (localState != NONE)) {
                // Only update the percentage value if it has changed
                LCD_Command(0xC0); // Move to second line
                LCD_String("Percent: ");
                LCD_Float(localPercentage);
                LCD_String("%");
            }

            // Store the current state and percentage for the next iteration
            lastState = localState;
            lastPercentage = localPercentage;
        }
            vTaskDelay(pdMS_TO_TICKS(90));
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



// ######################################################### //
// ######################### TEMPORARY TASKS ################# //
// ######################################################### //

void SuddenDisconnect_Task(void *pvParameters) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // SET FLAG
//        suddenDisconnect_flag = 1;
//        charging_state = STOP;
        PORTC |= (1 << PC7); // Set PC7 "Disconnected" to high
        vTaskDelay(pdMS_TO_TICKS(2000));
        PORTC &= ~(1 << PC7);  // Set PC7 "Disconnected" to low
//        suddenDisconnect_flag = 0;
//        charging_state = IDLE;
    }
}

// Overcurrent detection task
void Overcurrent_Task(void *pvParameters) {
    while (1) {
        // Wait for the overcurrent flag to be set
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//        overcurrent_flag = 1;

        // Stop Charging
//        charging_state = STOP;

        PORTC |= (1 << PC0); // Set PC0 to high
        vTaskDelay(pdMS_TO_TICKS(1000));
        PORTC &= ~(1 << PC0);  // Set PC0 to low

//        overcurrent_flag = 0;
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
