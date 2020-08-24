/*
 *	Queue.h
 *	Created: 1/12/2013 2:33:03 PM
 *	Modified by Nicholas Smith, Sean Robb
 */ 

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct{
	int8_t headIndex;
	int8_t tailIndex;
	void* buffer[8];
	}Queue;
	
#define wrap8(x)((x) % 8)
void Q_reset(Queue *q);

void Q_enqueue(void* x, Queue *q);

void* Q_lookIn(Queue *q);

static inline bool Q_eraseHead(Queue *q);

void* Q_dequeue(Queue *q);

// INSTEAD OF 2 MACROS, LET'S *DEFINE* 2 STATIC INLINE FUNCTIONS
// Any c-file including this header will get these just like a macro
// Inline is as fast as a macro in GCC, but gives cleaner error
// messages if we make a mistake in the use or definition.

static inline bool Q_isFull(Queue *q)
	{return (q->headIndex == (wrap8(q->tailIndex + 1)));}
		
static inline bool Q_isEmpty(Queue *q)
	{return (q->headIndex == q->tailIndex);}

#endif /* QUEUE_H_ */