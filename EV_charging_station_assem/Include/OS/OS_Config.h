/*
 * OS_Config.h
 *
 *  Created on: Jan 28, 2025
 *      Author: Horizon
 */

#ifndef INCLUDE_OS_OS_CONFIG_H_
#define INCLUDE_OS_OS_CONFIG_H_

#include "FreeRTOS.h"
#include "task.h"  
#include "semphr.h"
#include "queue.h" 
#include "OS_Config.h"




#define VEHICLE_CONNECTION_CHECK_PIN 2
#define VEHICLE_OVERCURRENT_CHECK_PIN 1


// Define system states
typedef enum {
    IDLE,
    READY,
    CHARGING,
    DISCHARGING,
    FAULT_DETECTION
} SystemState;



// FLAGS

uint8_t is_connected;
uint8_t is_sudden_disconnect;
uint8_t is_overcurrent;




// Global variable for system state
volatile SystemState globalState; // Default state


TaskHandle_t TaskChargingStartHandle, TaskStopSessionHandle, taskHandleDisplayRefresh, TaskUserCommandHandle, TaskFaultDetectionHandle;


TaskHandle_t SuddenDisconnect_TaskHandle;
TaskHandle_t Overcurrent_TaskHandle;
TaskHandle_t VehicleCheckTaskHandle;



// Queue Handle
QueueHandle_t buttonQueue;
// Semaphores and Task Handle
SemaphoreHandle_t lcdSemaphore;
SemaphoreHandle_t stateSemaphore;





#endif /* INCLUDE_OS_OS_CONFIG_H_ */
