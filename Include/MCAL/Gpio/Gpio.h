/*
 * Gpio.h
 *
 * Created: 05/01/2025
 * Author: Ahmed
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>

// Define GpioProxy structure
typedef struct {
	volatile uint8_t *portDir;  // Pointer to the port direction register
	volatile uint8_t *port;     // Pointer to the port data register
} GpioProxy;

// Define PinConfig structure for configuring pins in a modular way
typedef struct {
	GpioProxy *gpioProxy;   // GPIO proxy for the port
	uint8_t pin;            // Pin number
	uint8_t direction;      // Direction (0 for input, 1 for output)
	uint8_t value;          // Value (0 for low, 1 for high)
} PinConfig;

GpioProxy CreateGpioProxy(volatile uint8_t *portDir, volatile uint8_t *port);
void Proxy_SetPortDirection(GpioProxy *gpioProxy, uint8_t direction);
void Proxy_WritePort(GpioProxy *gpioProxy, uint8_t value);
uint8_t Proxy_ReadPort(GpioProxy *gpioProxy);
void Proxy_SetPin(GpioProxy *gpioProxy, uint8_t pin);
void Proxy_ClearPin(GpioProxy *gpioProxy, uint8_t pin);
uint8_t Proxy_ReadPin(GpioProxy *gpioProxy, uint8_t pin);
void ConfigurePins(void);

#endif /* GPIO_H_ */

