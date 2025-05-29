/*
 * Gpio_Config.h
 *
 * Created: 24/01/2025 06:20:01 م
 *  Author: Ahmed
 */ 


#ifndef GPIO_CFG_H_
#define GPIO_CFG_H_

#include "Gpio.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// Extern declarations for GpioProxy instances
extern GpioProxy lcdDataProxy;    // Proxy for LCD data port
extern GpioProxy lcdControlProxy; // Proxy for LCD control port
extern GpioProxy chargeProxy;

// Definitions for LCD control pins
#define RS 0 // RS pin index
#define EN 1 // EN pin index
#define NUM_INDECIES 6

// Extern declaration for the pin configurations array
extern PinConfig pinConfigs[]; // Array of PinConfig structures for LCD pins

#endif /* GPIO_CFG_H_ */
