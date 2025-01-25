/*
 * Logic.c
 *
 * Created: 24/01/2025 04:51:51 م
 *  Author: Ahmed
 */ 
#include "Logic.h"
#include "Gpio.h"
#include "Gpio_Config.h"

void Charge_Logic(void){

    // Set PC0 as output
    DDRC |= (1 << PC0);

    // Set PC0 to HIGH
    PORTC |= (1 << PC0);
}

void Stop_Charge_Logic(void){

	DDRA &= ~(1<<PA1);
	PORTA &= ~(1<<PA1);

	// Set PC0 as output
	DDRC |= (1 << PC1);

	// Set PC0 to HIGH
	PORTC |= (1 << PC1);
}
