/*
 * obsever.c
 *
 *  Created on: Jan 26, 2025
 *      Author: Aya
 */

#include "observer.h"
#include <stdio.h>

static SubscriberFunc subscribers[MAX_SUBSCRIBERS];		/* An array of function pointers to store the clients' callback functions.*/
static int subscriber_count = 0;						/* Keeps track of how many clients have subscribed.*/

/**
 * @brief Subscribes a client to the observer.
 *
 * @param func Pointer to the client's callback function.
 */
void subscribe(SubscriberFunc func) {
	/* if there is space in the subscribers array and the function pointer (func) is valid.*/
    if (subscriber_count < MAX_SUBSCRIBERS && func != NULL) {
    	/* stores the function pointer in the array */
        subscribers[subscriber_count++] = func;
    }
}

/**
 * @brief Notifies all subscribed clients by calling their callback functions.
 */
void notify(void) {
	/* Iterates through the subscribers array. */
    for (int i = 0; i < subscriber_count; i++) {
    	/* If a valid function pointer is found, it calls the function.*/
        if (subscribers[i] != NULL) {
            subscribers[i]();
        }
    }
}

/**
 * @brief Unsubscribes a client from the observer.
 *
 * @param func Pointer to the client's callback function to be removed.
 */
void unsubscribe(SubscriberFunc func) {
	/* Loop on the unsubscriber function pointer (func) in the subscribers array.*/
    for (int i = 0; i < subscriber_count; i++) {
        if (subscribers[i] == func) {
            // Remove the subscriber and shift others (remove the gaps of unsubscriber in the array)
            for (int j = i; j < subscriber_count - 1; j++) {
                subscribers[j] = subscribers[j + 1];
            }
            subscribers[--subscriber_count] = NULL;
            break;
        }
    }
}
