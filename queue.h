

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "my402list.h"
#include "cs402.h"






typedef struct Queue{

	My402List queue;

}Queue;





//Setting up the queue
extern void queueInit(Queue*); 
//Check if the queue is empty
extern int queueEmpty(Queue*);
//pops an element from the que and returns it 
extern void* queuePop(Queue*);
//Pushes an element into the queue.
extern int queuePush(Queue*, void*);
//Peaks into the first element in the queue and returns a constant referecne to it.
extern const void* queuePeek(Queue*);
extern void cleanQueue(Queue*s);

#endif /*_QUEUE_H_*/
