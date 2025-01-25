#include <avr/io.h>
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer_cfg.h"
// Global Variables
#define NULLPtr ((void *)0)
static void (*Timer1Callback)(void) = NULLPtr;
#define F_CPU 8000000UL


#define OCR1_VALUE (((F_CPU * time_ms) / (64U * 1000U)) - 1U)

// Timer1 Functions
void Timer_vidInitTimer1(void) {
	TCCR1A = 0;
	TCCR1B = (1 << WGM12); // CTC mode
	OCR1A = (uint16)OCR1_VALUE; // Set OCR1A value
	TIMSK = (1 << OCIE1A); // Enable Timer1 compare interrupt
	sei(); // Enable global interrupts
}

void Timer_vidStartTimer1(void) {
	#if PRESCALER == PRESCALER_1
	TCCR1B |= (1 << CS10);
	#elif PRESCALER == PRESCALER_8
	TCCR1B |= (1 << CS11);
	#elif PRESCALER == PRESCALER_64
	TCCR1B |= (1 << CS11) | (1 << CS10);
	#elif PRESCALER == PRESCALER_256
	TCCR1B |= (1 << CS12);
	#else
	#error "Invalid prescaler value"
	#endif
}

void Timer_vidStopTimer1(void) {
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void Timer_vidSetCallBackTimer1(void (*TimerCallback)(void)) {
	if (TimerCallback != NULLPtr) {
		Timer1Callback = TimerCallback;
	}
}

// ISR for Timer1 Compare Match
ISR(TIMER1_COMPA_vect) {
	if (Timer1Callback != NULLPtr) {
		(*Timer1Callback)(); // Call the user callback function
	}
}

