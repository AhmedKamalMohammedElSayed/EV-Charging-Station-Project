/*
 * Uart.h
 *
 * Created: 24/01/2025 04:28:42 م
 *  Author: Ahmed
 */ 


void UART_Init(unsigned int baud);
void UART_Transmit(char data);
void UART_SendString(const char *str);
void UART_Print(const char *str);
void UART_PrintFloat(float value);
