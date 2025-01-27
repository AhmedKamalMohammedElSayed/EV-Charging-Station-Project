#include "Logic.h"
#include "Gpio.h"
#include "Gpio_Config.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include <math.h>

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




/**
 * @brief Calculate the charge percentage based on the current voltage.
 * @param voltage The current voltage of the capacitor.
 * @return Charge percentage (0 to 100).
 */
float calculateChargePercentage(float voltage) {
    if (voltage < MIN_VOLTAGE) {
        return 0.0; // Below minimum voltage
    } else if (voltage > MAX_VOLTAGE) {
        return 100.0; // Above maximum voltage
    }
    return ((voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.0;
}

/**
 * @brief Calculate the time remaining to charge the capacitor to a target voltage.
 * @param currentVoltage The current voltage of the capacitor.
 * @param targetVoltage The target voltage to reach.
 * @param chargingCurrent The current used for charging (in amps).
 * @param capacitorValue The capacitance value of the capacitor (in farads).
 * @return Time remaining for charging (in seconds).
 */
float calculateTimeRemaining(float currentVoltage, float targetVoltage, float chargingCurrent, float capacitorValue) {
    if (currentVoltage >= targetVoltage || chargingCurrent <= 0 || capacitorValue <= 0) {
        return 0.0; // No time remaining if already charged or invalid input
    }

    // Calculate equivalent resistance (Ohm's law)
    float resistance = MAX_VOLTAGE / chargingCurrent;

    // Calculate the remaining time for charging
    float timeRemaining = -resistance * capacitorValue * log(1.0 - (targetVoltage / MAX_VOLTAGE));

    return timeRemaining; // Time in seconds
}
