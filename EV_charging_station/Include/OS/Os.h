
/*
 * Os.h
 *
 * Created: 24/01/2025 07:40:12 م
 *  Author: Ahmed
 */ 
#ifndef OS_H_
#define OS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"

SemaphoreHandle_t chargingSemaphore;


EventGroupHandle_t eventGroup;

// Define event bit masks
#define EVENT_CHARGING_DONE (1 << 0) // Bit 0 for Charging Done


TaskHandle_t TaskChargingStartHandle, TaskStopSessionHandle, TaskDisplayRefreshHandle, TaskUserCommandHandle, TaskFaultDetectionHandle;

void TaskChargingStart(void *pvParameters);
void TaskStopSession(void *pvParameters);
void TaskDisplayRefresh(void *pvParameters);
void TaskUserCommand(void *pvParameters);
void TaskFaultDetection(void *pvParameters);
void Os_Init(void);

#endif /* OS_H_ */
