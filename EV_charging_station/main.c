#include "ADC.h"
#include "ADC_Config.h"
#include <avr/io.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define IDLE 0
#define CHARGING 1
#define STOP 2



#define prescaler 128

#define ADC_THRESHOLD 100
#define CONNECTION_THRESHOLD 1022

uint8_t is_vehicle_connected = 0;

TaskHandle_t xADC_Task_Handle = NULL;
TaskHandle_t xOverCurrent_Task_Handle = NULL;

volatile uint8_t overcurrent_flag = 0;

#define VEHICLE_CHECK_PERIOD 100 // in ms
#define VEHICLE_CHECK_PIN 2

uint8_t charging_state = 0;

// Function to initialize UART for debugging (optional)
void UART_Init(unsigned int baud) {
    unsigned int ubrr = (F_CPU / 16 / baud) - 1;
    UBRRH = (unsigned char)(ubrr >> 8);  // Set high byte of baud rate
    UBRRL = (unsigned char)ubrr;        // Set low byte of baud rate
    UCSRB = (1 << TXEN);                // Enable transmitter
    UCSRC = (1 << URSEL) | (3 << UCSZ0); // 8-bit data, 1 stop bit
}

// Function to send a character via UART
void UART_Transmit(char data) {
    while (!(UCSRA & (1 << UDRE)));  // Wait for the buffer to be empty
    UDR = data;                     // Send the character
}

// Function to send a string via UART
void UART_SendString(const char *str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}

// ADC reading task
void ADC_Task(void *pvParameters) {
    uint16_t adc_value = 0;  // Raw ADC value
    uint16_t adc_OC_value = 0;  // Raw ADC value from overcurrent sensor

    while (1) {
        // Read the ADC value from channel 0 (ADC0)
        adc_value = ADC_READ(0);

        // Read the ADC value from channel 1 (ADC1)
        adc_OC_value = ADC_READ(1);

        adc_vehicle_connected = ADC_READ(VEHICLE_CHECK_PIN);

        // Check if the ADC value is above the threshold
        if (adc_OC_value > ADC_THRESHOLD) {
            // Set the overcurrent flag
            xTaskNotifyGive(xOverCurrent_Task_Handle);
        }

        // Check if a load is plugged in
        if (adc_vehicle_connected < CONNECTION_THRESHOLD) {
            // set vehicle connected flag
            is_vehicle_connected = 1;
        } else {
            is_vehicle_connected = 0;
        }

        // Save ADC value into shared memory (could be used in UART task)

        // Task delay for 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// UART communication task
void UART_Task(void *pvParameters) {
    uint16_t adc_value = 0;
    float voltage = 0;
    char buffer[32];
//    float test_val = 3.3;
    while (1) {
        // Retrieve the ADC value (could be shared from ADC task via queue/semaphore)
        adc_value = ADC_READ(0);
        voltage = ADC_GetVoltage(adc_value);


        // Format and send the values via UART
        snprintf(buffer, sizeof(buffer), "ADC: %d, Voltage: ", adc_value);
        UART_SendString(buffer);
        dtostrf(voltage, -6, 2, buffer);
        UART_SendString(buffer);
        UART_SendString(" V\r\n");

        adc_value = ADC_READ(1);
        voltage = ADC_GetVoltage(adc_value);


        // Format and send the values via UART
        snprintf(buffer, sizeof(buffer), "ADC_OC: %d, Voltage: ", adc_value);
        UART_SendString(buffer);
        dtostrf(voltage, -6, 2, buffer);
        UART_SendString(buffer);
        UART_SendString(" V\r\n");
        // Task delay for 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


// Overcurrent detection task
void Overcurrent_Task(void *pvParameters) {
    while (1) {
        // Wait for the overcurrent flag to be set
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        overcurrent_flag = 1;

        PORTC |= (1 << PC0); // Set PC0 to high
        vTaskDelay(pdMS_TO_TICKS(1000));
        PORTC &= ~(1 << PC0);  // Set PC0 to low

        overcurrent_flag = 0;
    }
}

void prd_VehicleCheck(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount(); 
    
    uint16_t adc_load_value = 0;
    
    while (1) {
        

        // Check if the ADC value is above the threshold
        if (adc_load_value < CONNECTION_THRESHOLD) {
            is_vehicle_connected = 1;
        } else {
            is_vehicle_connected = 0;
        }

        

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(VEHICLE_CHECK_PERIOD));
    }
}


void prd_LED_MANAGEMENT(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount(); 
    while (1) {
        if (is_vehicle_connected) {
            PORTC |= (1 << PC0); // Set PC0 to high
        } else {
            PORTC &= ~(1 << PC0);  // Set PC0 to low
        }

        switch (charging_state) {
            case IDLE:
                PORTC &= ~(1 << PC0);  // Set PC0 to low
                break;
            case CHARGING:
                PORTC |= (1 << PC0); // Set PC0 to high
                break;
            case STOP:
                PORTC &= ~(1 << PC0);  // Set PC0 to low
                break;
            default:
                break;
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
    }
}


void prd_Buttons(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount(); 
    while (1) {
        // If start is pressed
        if(is_vehicle_connected)
        if (PINB & (1 << PB0)) {
            
        }
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
    }
}

int main(void) {
    // Initialize the ADC and UART
    ADC_INIT(prescaler, AREF);
    UART_Init(9600);

    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5); // Set PC0 as output

    // Create the ADC reading task
    xTaskCreate(ADC_Task, "ADC Task", 128, NULL, 1, &xADC_Task_Handle);

    // Create the UART communication task
    xTaskCreate(UART_Task, "UART Task", 128, NULL, 2, NULL);

    // Create the overcurrent detection task
    xTaskCreate(Overcurrent_Task, "Overcurrent Task", 128, NULL, 1, &xOverCurrent_Task_Handle);

    // Start the scheduler to begin executing tasks
    vTaskStartScheduler();

    // If the scheduler fails to start, we'll end up here
    while (1) {}

    return 0;
}
