/*
 * Lcd.h
 *
 * Created: 05/01/2025 10:07:02 م
 *  Author: Ahmed
 */ 

#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCD_Dir  DDRB
#define LCD_Port PORTB
#define RS PB0
#define EN PB1

// Function Prototypes
void LCD_Init (void);
void LCD_Command( unsigned char cmnd );
void LCD_Char( unsigned char data );
void LCD_String (char *str);
void LCD_String_xy (char row, char pos, char *str);
void LCD_Clear();
#endif