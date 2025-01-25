#include "ADC.h"
#include "ADC_Config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define prescaler 128
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

int main(void) {
    // Initialize the ADC and UART
	ADC_INIT(prescaler, AREF);
    UART_Init(9600);

    // Variables
    uint16_t adc_value = 0;  // Raw ADC value
    float voltage = 0;       // Calculated voltage
    char buffer[32];         // Buffer for sending text over UART

    // Main loop
    while (1) {
        // Read the ADC value from channel 0 (ADC0)
        adc_value = ADC_READ(0);
        // Convert ADC value to voltage
		voltage = ADC_GetVoltage(adc_value);

		// Format and send the values via UART
		snprintf(buffer, sizeof(buffer), "ADC: %d\r\n", adc_value );
		UART_SendString(buffer);

//        DDRA |= (1<< PA7);
//        PORTA |= (1 << PA7);

        // Delay for readability
        _delay_ms(500);
    }

    return 0;
}
