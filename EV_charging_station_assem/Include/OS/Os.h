
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
#include "poller.h"


#define START_BUTTON_EVENT 1
#define STOP_BUTTON_EVENT 2
#define DEBOUNCE_TIME_MS 20
#define TASK_PERIOD pdMS_TO_TICKS(1000) // 1000 ms = 1 second


extern float cap_percentage ;
float percentage;






void TaskChargingStart(void *pvParameters);
void TaskStopSession(void *pvParameters);
void TaskDisplayRefresh(void *pvParameters);
void TaskUserCommand(void *pvParameters);
void TaskFaultDetection(void *pvParameters);
void Os_Init(void);

#endif /* OS_H_ */
