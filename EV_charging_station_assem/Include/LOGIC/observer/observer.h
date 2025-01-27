/*
 * obsever.h
 *
 *  Created on: Jan 26, 2025
 *      Author: Aya
 */

#ifndef OBSEVER_OBSEVER_H_
#define OBSEVER_OBSEVER_H_

#define MAX_SUBSCRIBERS 5					/* Defines the maximum number of clients that can subscribe to the observer. */

typedef void (*SubscriberFunc)(void);


void subscribe(SubscriberFunc func);
void notify(void);
void unsubscribe(SubscriberFunc func);


#endif /* OBSEVER_OBSEVER_H_ */
