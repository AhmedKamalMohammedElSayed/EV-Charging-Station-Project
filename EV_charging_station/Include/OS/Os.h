
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
#include "queue.h"


#define START_BUTTON_EVENT 1
#define STOP_BUTTON_EVENT 2
#define DEBOUNCE_TIME_MS 20

// Queue Handle
QueueHandle_t buttonQueue;


TaskHandle_t TaskChargingStartHandle, TaskStopSessionHandle, TaskDisplayRefreshHandle, TaskUserCommandHandle, TaskFaultDetectionHandle;

void TaskChargingStart(void *pvParameters);
void TaskStopSession(void *pvParameters);
void TaskDisplayRefresh(void *pvParameters);
void TaskUserCommand(void *pvParameters);
void TaskFaultDetection(void *pvParameters);
void Os_Init(void);

#endif /* OS_H_ */
