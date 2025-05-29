/*
 * Gpio_cfg.c
 *
 * Created: 05/01/2025
 * Author: Ahmed
 */

#include "Gpio.h"
#include "Gpio_Config.h"


// Declare GpioProxy instances for the required ports
GpioProxy lcdDataProxy = {&DDRB, &PORTB}; // Proxy for LCD data port (e.g., PORTC)
GpioProxy lcdControlProxy = {&DDRB, &PORTB}; // Proxy for LCD control port (e.g., PORTD)
GpioProxy chargeProxy ={&DDRD, &PORTD};
	
// Define the pins for RS, EN, and data lines
#define RS 0 // RS pin on PORTD, Pin 0
#define EN 1 // EN pin on PORTD, Pin 1

// Array of PinConfig structures for the LCD
PinConfig pinConfigs[] = {
    // {GpioProxy*, Pin, Direction, Value}
    {&lcdControlProxy, RS, 1, 0}, // RS pin as output, initialized to low
    {&lcdControlProxy, EN, 1, 0}, // EN pin as output, initialized to low
    {&lcdDataProxy, 4, 1, 0},     // Data pins (D4-D7) as output, initialized to low
    {&lcdDataProxy, 5, 1, 0},
    {&lcdDataProxy, 6, 1, 0},
    {&lcdDataProxy, 7, 1, 0},
//	{&chargeProxy,0,1,1}
};
