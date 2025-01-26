#include "ADC.h"
#include "ADC_Config.h"
#include <avr/io.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define IDLE 0
#define READY 1
#define CHARGING 2
#define STOP 3
#define PAUSED 4



#define prescaler 128

#define ADC_THRESHOLD 25
#define CONNECTION_THRESHOLD 900

volatile uint8_t is_vehicle_connected = 0;

TaskHandle_t xADC_Task_Handle = NULL;
TaskHandle_t xVehicleCheck_Task_Handle = NULL;
TaskHandle_t xOverCurrent_Task_Handle = NULL;
TaskHandle_t xSuddenDisconnect_Task_Handle = NULL;

volatile uint8_t overcurrent_flag = 0;
volatile uint8_t suddenDisconnect_flag = 0;

#define VEHICLE_CHECK_PERIOD 100 // in ms
#define VEHICLE_CHECK_PIN 2

volatile uint8_t charging_state = IDLE;
volatile uint8_t prev_charging_state = IDLE;

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
    static uint16_t adc_value = 0;  // Raw ADC value
    static uint16_t adc_OC_value = 0;  // Raw ADC value from overcurrent sensor
    static uint16_t adc_vehicle_connected = 0;  // Raw ADC value from overcurrent sensor

    while (1) {
        // Read the ADC value from channel 0 (ADC0)
        adc_value = ADC_READ(0);

        // Read the ADC value from channel 1 (ADC1)
        adc_OC_value = ADC_READ(1);

        // adc_vehicle_connected = ADC_READ(VEHICLE_CHECK_PIN);

        // Check if the ADC value is above the threshold
        if (adc_OC_value > ADC_THRESHOLD) {
            // Set the overcurrent flag
            xTaskNotifyGive(xOverCurrent_Task_Handle);
        }

        // Task delay for 500ms
        vTaskDelay(pdMS_TO_TICKS(300));
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
        snprintf(buffer, sizeof(buffer), "Charging state: %d \r\n", charging_state);

        UART_SendString(buffer);
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

        // Stop Charging
        charging_state = STOP;

        PORTC |= (1 << PC0); // Set PC0 to high
        vTaskDelay(pdMS_TO_TICKS(1000));
        PORTC &= ~(1 << PC0);  // Set PC0 to low

        overcurrent_flag = 0;
    }
}

void SuddenDisconnect_Task(void *pvParameters) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // SET FLAG
        suddenDisconnect_flag = 1;
        charging_state = STOP;
        PORTC |= (1 << PC7); // Set PC7 "Disconnected" to high
        vTaskDelay(pdMS_TO_TICKS(2000));
        PORTC &= ~(1 << PC7);  // Set PC7 "Disconnected" to low
        suddenDisconnect_flag = 0;
        charging_state = IDLE;
    }
}



void prd_VehicleCheck(void *pvParameters) {
    // TickType_t xLastWakeTime = xTaskGetTickCount(); 
    
    static uint16_t adc_load_value = 0;
    
    while (1) {
        
        adc_load_value = ADC_READ(VEHICLE_CHECK_PIN);


        // Check if the ADC value is above the threshold
        if (adc_load_value < CONNECTION_THRESHOLD) {
            is_vehicle_connected = 1;
            if (charging_state == IDLE) {
                
                charging_state = READY;
                
            }
        
        } else {
            is_vehicle_connected = 0;
            if ((charging_state == CHARGING) | (charging_state == PAUSED) ) {
                
                // charging_state = STOP;
                xTaskNotifyGive(xSuddenDisconnect_Task_Handle);
            } else {
                charging_state = IDLE;
            }
        }

        // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(VEHICLE_CHECK_PERIOD));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


void prd_LED_MANAGEMENT(void *pvParameters) {
    // TickType_t xLastWakeTime = xTaskGetTickCount(); 
    while (1) {

        switch (charging_state) {
            case IDLE:
                PORTC |= (1 << PC1);  // Set 'Idle' LED to high
        
                PORTC &= ~(1 << PC2);  // Set 'Ready' LED to low
                PORTC &= ~(1 << PC3);  // Set 'Charging' LED to low
                PORTC &= ~(1 << PC4);  // Set 'Stop' LED to low
                PORTC &= ~(1 << PC6); // Set PC6 to low
                break;
                
            case READY:
                PORTC |= (1 << PC2);  // Set 'Ready' LED to high
                PORTC &= ~(1 << PC1);  // Set 'Idle' LED to low
                PORTC &= ~(1 << PC3);  // Set 'Charging' LED to low
                PORTC &= ~(1 << PC4);  // Set 'Stop' LED to low
                PORTC &= ~(1 << PC6); // Set PC6 to low
                break;

            case CHARGING:
                PORTC |= (1 << PC5); // Set PC5 to high

                PORTC |= (1 << PC3);  // Set 'Charging' LED to high 
                PORTC &= ~(1 << PC1);  // Set 'Idle' LED to low
                PORTC &= ~(1 << PC2);  // Set 'Ready' LED to low
                PORTC &= ~(1 << PC4);  // Set 'Stop' LED to low
                PORTC &= ~(1 << PC6); // Set PC6 to low
                break;

            case PAUSED:
                PORTC |= (1 << PC6); // Set PC6 to high

                PORTC &= ~(1 << PC5); // Set PC5 to low

                PORTC &= ~(1 << PC3);  // Set 'Charging' LED to low
                PORTC &= ~(1 << PC1);  // Set 'Idle' LED to low
                PORTC &= ~(1 << PC2);  // Set 'Ready' LED to low
                PORTC &= ~(1 << PC4);  // Set 'Stop' LED to low
                break;

            case STOP:
                PORTC &= ~(1 << PC5); // Set PC5 to low

                PORTC |= (1 << PC4);  // Set 'Stop' LED to high
                PORTC &= ~(1 << PC2);  // Set 'Ready' LED to low
                PORTC &= ~(1 << PC3);  // Set 'Charging' LED to low
                PORTC &= ~(1 << PC1);  // Set 'IDLE' LED to low
                PORTC &= ~(1 << PC6); // Set PC6 to low
                
                break;
            default:
                break;
        }
        // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


void prd_Buttons(void *pvParameters) {
    // TickType_t xLastWakeTime = xTaskGetTickCount(); 
    while (1) {
        // PIND |= (1 << PD2); //toggle PD2
        if(charging_state != IDLE){
            
            // If start is pressed
            if (PINB & (1 << PB0)) {
                charging_state = CHARGING;
            }

            // If stop is pressed
            if (PINB & (1 << PB1)) {
                charging_state = STOP;
            }

            if (PINB & (1 << PB2)) {
                charging_state = PAUSED;
            }

        }
        vTaskDelay(pdMS_TO_TICKS(200));
        // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}


int main(void) {
    // Initialize the ADC and UART
    ADC_INIT(prescaler, AREF);
    UART_Init(9600);

    // DDRC |= (0x7F); // Set PC0-7 as output
    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7); // Set PC0-7 as output
    DDRD |= (1 << PD2); // Set PD2 as output
    // Create the ADC reading task
    xTaskCreate(ADC_Task, "ADC Task", 128, NULL, 1, &xADC_Task_Handle);

    // Create the UART communication task
    xTaskCreate(UART_Task, "UART Task", 128, NULL, 5, NULL);

    // Create the overcurrent detection task
    xTaskCreate(Overcurrent_Task, "Overcurrent Task", 128, NULL, 1, &xOverCurrent_Task_Handle);

    // Create the sudden disconnect detection task
    xTaskCreate(SuddenDisconnect_Task, "Sudden Disconnect Task", 128, NULL, 2, &xSuddenDisconnect_Task_Handle);

    // Create the LED management task
    xTaskCreate(prd_LED_MANAGEMENT, "LED Management Task", 128, NULL, 4, NULL);

    // Create the vehicle check task
    xTaskCreate(prd_VehicleCheck, "Vehicle Check Task", 128, NULL, 2, &xVehicleCheck_Task_Handle);

    // Create the buttons task
    xTaskCreate(prd_Buttons, "Buttons Task", 128, NULL, 3, NULL);

    // Start the scheduler to begin executing tasks
    vTaskStartScheduler();

    // If the scheduler fails to start, we'll end up here
    while (1) {}

    return 0;
}
