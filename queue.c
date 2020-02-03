
#include "queue.h"

/*
	Initializes the queue to an empty queue
*/
void queueInit(Queue* q)
{
	My402List* list = &q->queue;
	My402ListInit(list);
}


int queueEmpty(Queue* q)
{
	if(My402ListEmpty(&q->queue))
	{
		return TRUE;
	}
	return FALSE;
}

/*
Returns the first Element in the queue if the queue is not empty and NULL otherwise
*/
void* queuePop(Queue* q)
{

	if(!queueEmpty(q))
	{
		My402ListElem* firstInList = My402ListFirst(&q->queue);
		void* item = firstInList->obj;
		My402ListUnlink(&q->queue, firstInList);
		return item;
	}
	return NULL;
}


/*
Tries to place an item in the que
*/
int queuePush(Queue* q, void* item)
{
	if(My402ListAppend(&q->queue, item))
	{
		return TRUE;
	}
	return FALSE;											
}


const void* queuePeek(Queue* q)
{
	if(!queueEmpty(q))
	{
		My402ListElem* firstInList = My402ListFirst(&q->queue);
		void* item = firstInList->obj;
		return item;
	}
	return NULL;
}


void cleanQueue(Queue* q)
{
	My402ListUnlinkAll(&q->queue);
	q = NULL;
}

