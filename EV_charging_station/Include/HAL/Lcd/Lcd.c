/*
 * Lcd.c
 *
 * Created: 05/01/2025
 * Author: Ahmed
 */ 

#include "lcd.h"
#include "Gpio.h"
#include <util/delay.h> // Include delay functions

// External GpioProxy instances for LCD ports
extern GpioProxy lcdDataProxy;     // Proxy for data pins (D4-D7)
extern GpioProxy lcdControlProxy; // Proxy for control pins (RS, EN)

// Pin definitions
#define RS 0  // RS pin connected to Pin 0 of lcdControlProxy
#define EN 1  // EN pin connected to Pin 1 of lcdControlProxy

void LCD_Command(unsigned char cmnd) {
    // Send upper nibble
    Proxy_WritePort(&lcdDataProxy, (Proxy_ReadPort(&lcdDataProxy) & 0x0F) | (cmnd & 0xF0));
    Proxy_ClearPin(&lcdControlProxy, RS); // RS=0, command register
    Proxy_SetPin(&lcdControlProxy, EN);  // Enable pulse
    _delay_us(1);
    Proxy_ClearPin(&lcdControlProxy, EN);
    _delay_us(200);

    // Send lower nibble
    Proxy_WritePort(&lcdDataProxy, (Proxy_ReadPort(&lcdDataProxy) & 0x0F) | (cmnd << 4));
    Proxy_SetPin(&lcdControlProxy, EN);
    _delay_us(1);
    Proxy_ClearPin(&lcdControlProxy, EN);
    _delay_ms(2);
}

void LCD_Char(unsigned char data) {
    // Send upper nibble
    Proxy_WritePort(&lcdDataProxy, (Proxy_ReadPort(&lcdDataProxy) & 0x0F) | (data & 0xF0));
    Proxy_SetPin(&lcdControlProxy, RS);  // RS=1, data register
    Proxy_SetPin(&lcdControlProxy, EN); // Enable pulse
    _delay_us(1);
    Proxy_ClearPin(&lcdControlProxy, EN);
    _delay_us(200);

    // Send lower nibble
    Proxy_WritePort(&lcdDataProxy, (Proxy_ReadPort(&lcdDataProxy) & 0x0F) | (data << 4));
    Proxy_SetPin(&lcdControlProxy, EN);
    _delay_us(1);
    Proxy_ClearPin(&lcdControlProxy, EN);
    _delay_ms(2);
}

void LCD_Init(void) {
    // Set LCD port direction (D4-D7 as output, RS and EN as output)
    Proxy_SetPortDirection(&lcdDataProxy, 0xF0); // Data pins D4-D7 (upper nibble)
    Proxy_SetPortDirection(&lcdControlProxy, (1 << RS) | (1 << EN)); // RS and EN pins

    _delay_ms(20); // LCD Power ON delay always >15ms

    LCD_Command(0x02); // Initialize LCD in 4-bit mode
    LCD_Command(0x28); // 2 lines, 5x7 matrix in 4-bit mode
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Increment cursor (shift cursor to right)
    LCD_Command(0x01); // Clear display
    _delay_ms(2);
}

void LCD_String(char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_String_xy(char row, char pos, char *str) {
    if (row == 0 && pos < 16)
        LCD_Command((pos & 0x0F) | 0x80); // First row position
    else if (row == 1 && pos < 16)
        LCD_Command((pos & 0x0F) | 0xC0); // Second row position
    LCD_String(str);
}

void LCD_Clear(void) {
    LCD_Command(0x01); // Clear display
    _delay_ms(2);
    LCD_Command(0x80); // Cursor at home position
}

void LCD_Float(float value) {
	char buffer[10];
	dtostrf(value, 4, 2, buffer);  // Convert float to string with 2 decimal places
	LCD_String(buffer);  // Display the string on the LCD
}
