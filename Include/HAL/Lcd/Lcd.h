/*
 * Lcd.h
 *
 * Created: 05/01/2025
 * Author: Ahmed
 */

#ifndef LCD_H_
#define LCD_H_

#include "Gpio.h"

// Function prototypes

/**
 * @brief Initialize the LCD in 4-bit mode.
 * This function sets the LCD to 4-bit mode, configures its pins, and clears the display.
 */
void LCD_Init(void);

/**
 * @brief Send a command to the LCD.
 * @param cmnd The command byte to send.
 * 
 * This function sends a command to the LCD by toggling the RS and EN pins.
 */
void LCD_Command(unsigned char cmnd);

/**
 * @brief Display a character on the LCD.
 * @param data The character to display.
 * 
 * This function sends a single character to the LCD to display.
 */
void LCD_Char(unsigned char data);

/**
 * @brief Display a string on the LCD.
 * @param str Pointer to the string to display.
 * 
 * This function sends each character in the string to the LCD.
 */
void LCD_String(char *str);

/**
 * @brief Display a string at a specific position on the LCD.
 * @param row The row index (0 for the first row, 1 for the second row).
 * @param pos The column index (0 to 15).
 * @param str Pointer to the string to display.
 * 
 * This function positions the cursor at the specified row and column,
 * then displays the string.
 */
void LCD_String_xy(char row, char pos, char *str);

/**
 * @brief Clear the LCD display.
 * 
 * This function clears the LCD screen and sets the cursor to the home position.
 */
void LCD_Clear(void);



void LCD_Float(float value);

#endif /* LCD_H_ */
