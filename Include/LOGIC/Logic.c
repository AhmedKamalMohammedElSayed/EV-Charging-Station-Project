#include "Logic.h"
#include "Gpio.h"
#include "Gpio_Config.h"
#include "avr/io.h"
#include "avr/interrupt.h"

void Charge_Logic(void) {
	// Enable an alternative functionality on PC1
	DDRC &= ~(1 << PC1);  // Set PC1 as output
	PORTC &= ~(1 << PC1); // Turn on alternative indicator
    // Set PC0 as output and drive it HIGH
    DDRC |= (1 << PC0);  // Set PC0 as output
    PORTC |= (1 << PC0); // Turn on charging
}

void Stop_Charge_Logic(void) {
    // Stop charging by clearing PC0
    PORTC &= ~(1 << PC0); // Turn off charging
    DDRC &= ~(1 << PC0);  // Optionally, set PC0 as input (optional)

    // Enable an alternative functionality on PC1
    DDRC |= (1 << PC1);  // Set PC1 as output
    PORTC |= (1 << PC1); // Turn on alternative indicator
}



void Button_Init(void) {
    // Configure PD2 (INT0) and PD3 (INT1) as inputs
    DDRD &= ~((1 << PD2) | (1 << PD3)); // Set PD2 and PD3 as input
    PORTD |= (1 << PD2) | (1 << PD3);   // Enable internal pull-up resistors

    // Configure falling-edge interrupt for INT0 (PD2) and INT1 (PD3)
    MCUCR |= (1 << ISC01) | (1 << ISC11); // Falling edge triggers
    MCUCR &= ~((1 << ISC00) | (1 << ISC10)); // Ensure falling edge mode

    // Enable INT0 and INT1
    GICR |= (1 << INT0) | (1 << INT1);

    // Enable global interrupts
    sei();
}
