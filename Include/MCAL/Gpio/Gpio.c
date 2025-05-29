/*
 * gpio.c
 *
 * Created: 05/01/2025
 * Author: Ahmed
 */

#include "Gpio.h"
#include "Gpio_Config.h"

// Create a GpioProxy for a specific port (portDir and port are pointers to the direction and data registers)
GpioProxy CreateGpioProxy(volatile uint8_t *portDir, volatile uint8_t *port) {
    GpioProxy gpioProxy;
    gpioProxy.portDir = portDir;
    gpioProxy.port = port;
    return gpioProxy;
}

// Set port direction (0 for input, 1 for output)
void Proxy_SetPortDirection(GpioProxy *gpioProxy, uint8_t direction) {
    *(gpioProxy->portDir) = direction;
}

// Write a value to the port
void Proxy_WritePort(GpioProxy *gpioProxy, uint8_t value) {
    *(gpioProxy->port) = value;
}

// Read the value of the port
uint8_t Proxy_ReadPort(GpioProxy *gpioProxy) {
    return *(gpioProxy->port);
}

// Set a specific pin high
void Proxy_SetPin(GpioProxy *gpioProxy, uint8_t pin) {
    *(gpioProxy->port) |= (1 << pin);
}

// Clear a specific pin (set it low)
void Proxy_ClearPin(GpioProxy *gpioProxy, uint8_t pin) {
    *(gpioProxy->port) &= ~(1 << pin);
}

// Read the state of a specific pin
uint8_t Proxy_ReadPin(GpioProxy *gpioProxy, uint8_t pin) {
    return (*(gpioProxy->port) & (1 << pin)) >> pin;
}

// Function to configure pins based on the pinConfigs array
void ConfigurePins(void) {
    uint8_t i;
	uint8_t index_no = NUM_INDECIES;

    //uint8_t numPins = sizeof(pinConfigs) / sizeof(pinConfigs[0]);

    for (i = 0; i < index_no; i++) {
        PinConfig *pin = &pinConfigs[i];

        // Set pin direction
        if (pin->direction == 1) {
            Proxy_SetPortDirection(pin->gpioProxy, (1 << pin->pin));  // Set as output
        } else {
            Proxy_SetPortDirection(pin->gpioProxy, 0);  // Set as input
        }

        // Set pin value (only for output pins)
        if (pin->direction == 1) {
            if (pin->value == 1) {
                Proxy_SetPin(pin->gpioProxy, pin->pin);  // Set pin high
            } else {
                Proxy_ClearPin(pin->gpioProxy, pin->pin);  // Set pin low
            }
        } else {
            // For input pins, we can enable pull-up resistors if required
            if (pin->value == 1) {
                Proxy_SetPin(pin->gpioProxy, pin->pin);  // Enable pull-up resistor
            } else {
                Proxy_ClearPin(pin->gpioProxy, pin->pin);  // Disable pull-up resistor
            }
        }
    }
}
