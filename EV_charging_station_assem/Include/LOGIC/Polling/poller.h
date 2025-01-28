#ifndef POLLER_H
#define POLLER_H

#include "Std_Types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "OS_Config.h"


void poller_Init(void);
void Poller__StartPolling(void);
void Poller__StopPolling(void);
void poller_SetPollTime(uint32 time_ms);
void updatePollCounter(void);
void poll(void);

#endif
