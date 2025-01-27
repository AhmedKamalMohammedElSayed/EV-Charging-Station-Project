#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include "timer.h"
#include "timer_cfg.h"

// Global Variables
#define NULLPtr ((void *)0)
static TimerHandle_t xTimer1 = NULL;
static void (*Timer1Callback)(void) = NULLPtr;

static void vTimer1Callback(TimerHandle_t xTimer) {
    if (Timer1Callback != NULLPtr) {
        (*Timer1Callback)(); // Call the user callback function
    }
}

// Initialize Timer1 (FreeRTOS Software Timer)
void Timer_vidInitTimer1(void) {
    // Create the software timer
    xTimer1 = xTimerCreate(
        "Timer1",                             // Name of the timer
        pdMS_TO_TICKS(time_ms),               // Timer period in ticks
        pdTRUE,                               // Auto-reload (true for periodic timer)
        NULL,                                 // Timer ID (not used)
		vTimer1Callback                       // Callback function
    );


}

// Start Timer1 (FreeRTOS Software Timer)
void Timer_vidStartTimer1(void) {
    if (xTimer1 != NULL) {
        xTimerStart(xTimer1, 0);
    }
}

// Stop Timer1 (FreeRTOS Software Timer)
void Timer_vidStopTimer1(void) {
    if (xTimer1 != NULL) {
        xTimerStop(xTimer1, 0);
    }
}

// Set the callback function for Timer1
void Timer_vidSetCallBackTimer1(void (*TimerCallback)(void)) {
    if (TimerCallback != NULLPtr) {
        Timer1Callback = TimerCallback;
    }
}
