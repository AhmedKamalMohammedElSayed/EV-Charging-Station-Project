/*
 * Uart.c
 *
 * Created: 24/01/2025 04:28:22 م
 *  Author: Ahmed
 */ 
#include "Uart.h"
#include "avr/io.h"
#include <stdio.h>

#define F_CPU 8000000 
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



void UART_Print(const char *str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}


// UART Print Float Using dtostrf
void UART_PrintFloat(float value) {
    char buffer[16];
    dtostrf(value, 6, 2, buffer); // Convert float to string (width: 6, precision: 2)
    UART_Print(buffer);
}
