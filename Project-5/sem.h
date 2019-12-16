/*
Submission By

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID:1217164507

Semaphore_t structure will have count of the semaphore value and the queue of TCB_queue

P(): it will reduce the count value and if the semaphore count is less than 0 it will delete  the first element in ReadyQ and add that to the semaphore queue. After that it will swapcontext to next function in the ReadyQ

V(): It will increment the count value of semaphore and if semaphore value is less than equal to 0 it will delete the first element of the semaphore queue and add it to the end of the ReadyQ. Irrespective of the count value it will call yield function which will context switch to next function in the ReadyQ

*/



#include "threads.h"
#include <string.h>

typedef struct Semaphore_t {
	int count;
	node* TCB_queue;
} Semaphore_t;

Semaphore_t* CreateSem(int InputValue)
{
	Semaphore_t *newnode=(Semaphore_t *)malloc(sizeof(Semaphore_t));
	newnode->count=InputValue;
	newnode->TCB_queue=newQueue();
	return newnode;
}

void P(Semaphore_t **sem)
{
	TCB_t *tcb;
	(*sem)->count-=1;
	if((*sem)->count<0)
	{
		
		tcb=DelQueue(&ReadyQ);
		AddQueue(&(*sem)->TCB_queue,&tcb);
		swapcontext(&(tcb->context), &(ReadyQ->context));
	}
}

void V(Semaphore_t **sem)
{
	TCB_t *tcb;
	(*sem)->count+=1;
	if((*sem)->count<=0)
	{
		tcb=DelQueue(&(*sem)->TCB_queue);
		if(tcb!=NULL)
		{
			AddQueue(&ReadyQ,&tcb);
		}
	}
	yield();
}
