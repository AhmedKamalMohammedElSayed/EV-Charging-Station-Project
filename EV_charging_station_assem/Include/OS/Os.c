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

uint8_t is_connected = 0;
uint8_t is_sudden_disconnect = 0;
uint8_t is_overcurrent = 0;




// Global variable for system state
volatile SystemState globalState = IDLE; // Default state




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

	SystemState localState;

    for(;;) {

			DDRD |= (1 << PD6);

			PORTD ^= (1<<PD6);
		    LCD_Clear(); // Only clear when the state changes



            LCD_String("State:"); // Print label

            // Safely access the global system state
            if (xSemaphoreTake(stateSemaphore, pdMS_TO_TICKS(10)) == pdTRUE) {

                localState = globalState;
                xSemaphoreGive(stateSemaphore); // Release the state semaphore
            }
            // Update the display based on the current system state
            switch (localState) {
                case IDLE:
                    LCD_String("IDLE"); // Display NONE state
                    break;
                case READY:
                    LCD_String("READY");
                    break;
                case CHARGING:
                    LCD_String("Charge"); // Display CHARGING state
                    LCD_Command(0xC0); // Move to the second line
                    LCD_String("Percentage: ");
					LCD_Float(cap_percentage); // Display real-time capacity percentage
					LCD_String(" %");
                    break;
                case DISCHARGING:
                    LCD_String("Discharge"); // Display DISCHARGING state
                    LCD_Command(0xC0); // Move to the second line
                    LCD_String("Percentage: ");
                    LCD_Float(cap_percentage); // Display real-time capacity percentage
                    LCD_String(" %");
                    break;
                case FAULT_DETECTION:
                    LCD_String("FAULT"); // Display FAULT state
                    LCD_Command(0xC0); // Move to the second line
                    if (is_overcurrent){
                        LCD_String("OVERCURRENT!");
                    } else if (is_sudden_disconnect){
                        LCD_String("SUDDEN DISCONNECT");
                    }
                    break;
            }
            // Convert the period from ticks to milliseconds
			uint32_t periodMs = pdTICKS_TO_MS(xFrequency);

            vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}





void VehicleCheckTask(void *pvParameters) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    // TickType_t xLastWakeTime = xTaskGetTickCount(); 
    
    static uint16_t adc_OC_Check_value = 0;
    static uint16_t adc_connection_check = 0;
    SystemState local_state = globalState;
    
    while (1) {
        
        adc_connection_check = ADC_READ(VEHICLE_CONNECTION_CHECK_PIN);
        adc_OC_Check_value = ADC_READ(VEHICLE_OVERCURRENT_CHECK_PIN);


        // Check if the ADC value is above the threshold
        if (adc_connection_check < CONNECTION_THRESHOLD) {
            is_connected = 1;
            if ((globalState == IDLE)) {
                
                local_state = READY;
                
            }
        } else if (adc_connection_check > CONNECTION_THRESHOLD){
            is_connected = 0;
            if ((globalState == CHARGING)) {    
                // charging_state = STOP;
                // xTaskNotifyGive(xSuddenDisconnect_Task_Handle);
                is_sudden_disconnect = 1;
                local_state = FAULT_DETECTION;
            } else {
                local_state = IDLE;
            }
        } else if (adc_OC_Check_value > OVERCURRENT_THRESHOLD){
            is_overcurrent = 1;
            local_state = FAULT_DETECTION;

        }

        if (xSemaphoreTake(stateSemaphore, portMAX_DELAY) == pdTRUE) {
            if (globalState != local_state) {
                
                globalState = local_state;
                
            }
            xSemaphoreGive(stateSemaphore);
        }

        // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(VEHICLE_CHECK_PERIOD));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}




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

        PORTC |= (1 << PC7); // Set PC7 "Disconnected" to high
        vTaskDelay(pdMS_TO_TICKS(2000));
        PORTC &= ~(1 << PC7);  // Set PC7 "Disconnected" to low

    }
}

// Overcurrent detection task
void Overcurrent_Task(void *pvParameters) {
    while (1) {
        // Wait for the overcurrent flag to be set
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        is_overcurrent = 1;
        
        if (xSemaphoreTake(stateSemaphore, portMAX_DELAY) == pdTRUE) {
            globalState = FAULT_DETECTION;
            xSemaphoreGive(stateSemaphore);
        }
        // PORTC |= (1 << PC0); // Set PC0 to high
        vTaskDelay(pdMS_TO_TICKS(1000));
        // PORTC &= ~(1 << PC0);  // Set PC0 to low

    }
}











// OS Initialization
void Os_Init(void) {
    // Initialize peripherals
	LCD_Init();



    // Create binary semaphore for global state
    stateSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(stateSemaphore);

    // Create the button queue
    buttonQueue = xQueueCreate(5, sizeof(uint8_t));

//     Create tasks
    xTaskCreate(TaskDisplayRefresh, "DisplayRefresh", 128, NULL, 4, &taskHandleDisplayRefresh);
    xTaskCreate(TaskChargingStart, "ChargingStart", 128, NULL, 2, NULL);
    xTaskCreate(TaskStopSession, "StopSession", 128, NULL, 2, NULL);
    xTaskCreate(VehicleCheckTask, "VehicleCheck", 128, NULL, 2, &VehicleCheckTaskHandle);
    xTaskCreate(Overcurrent_Task, "Overcurrent Task", 128, NULL, 4, &Overcurrent_TaskHandle);
    
//    xTaskCreate(prd_Buttons, "prd_Buttons", 128, NULL, 4, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}
