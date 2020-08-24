/*
 *	Queue.c
 *	Created: 1/12/2013 2:32:55 PM
 *	Modified by Nicholas Smith, Sean Robb
 *	About: The header file for Soft_Timer.c that declares all needed struct and functions	
 */ 
/*simple queue of void pointers, used here for function pointers and for integer parameters
via casts...not a good idea but we will be careful and keep it simple,
Head points to the oldest entry, tail points to first empty space. This will be used later 
in integration with the battery tester. Original code credited to Dr. Sean M. Carroll*/

/*INDEX
Q_
	dequeue = remove & return head (=lookIn + eraseHead)
	enqueue = add to tail
	eraseHead = remove but do not return (returns success flag)
	resetPtrs = reset head and tail pointers
	isFull = inline bool
	isEmpty = inline bool
	lookIn = return head but no side effect on queue data or pointers
	prepend = push onto head (undoes "dequeue")
*/
#include "Queue.h"

void* Q_dequeue(Queue *q)
{
	
	void* retVal;
	if(Q_isEmpty(q))return (void*)-1;
	//Is there data in Queue? If so copy it out
	retVal = q->buffer[q->headIndex];
	//and increment the head index up around ring buffer
	q->headIndex = wrap8(q->headIndex+1);
	return retVal;
}

static inline bool Q_eraseHead(Queue *q)
{
	if(Q_isEmpty(q)) return false;
	int n = q->headIndex = wrap8(n+1);
	return true;
}

void Q_enqueue(void* x, Queue *q)
{
	if(Q_isFull(q)) return;
	//Is there room in the Queue? then append x to ring buffer
	q->buffer[q->tailIndex] = x;
	//increment the tail index
	q->tailIndex = wrap8(q->tailIndex + 1); 	
}

void Q_resetPtrs(Queue *q)
{
	q->headIndex = 0;
	q->tailIndex = 0;
	
}

void* Q_lookIn(Queue *q)
{
	if(Q_isEmpty(q)) return (void*)-1;
	return q->buffer[q->headIndex];	
}