#ifndef TIMER_H
#define TIMER_H

#include "Std_Types.h"
#include "FreeRTOS.h"
#include "timers.h"

// Function prototype for the timer callback
void Timer_vidSetCallBackTimer1(void (*TimerCallback)(void));

typedef void (*VoidTimerCallback)(void);

void Timer_vidInitTimer1(void);
void Timer_vidStartTimer1(void);
void Timer_vidStopTimer1(void);
//void Timer_vidSetCallBackTimer1(VoidTimerCallback TimerCallback);


extern volatile uint16 compare_value;


#endif
