// (c) Partha Dasgupta 2009
// permission to use and distribute granted.


/*
Submission By

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID:1217164507

ReadyQ points to a queue of TCBs and Curr_Thread points to the thread under execution

As mentioned in the project the below functions were created

void start_thread(void (*function)(void))
{ // begin pseudo code
     allocate a stack (via malloc) of a certain size (choose 8192)
     allocate a TCB (via malloc)
     call init_TCB with appropriate arguments
     Add a thread_id (use a counter)
     call addQ to add this TCB into the “ReadyQ” which is a global header pointer
  //end pseudo code
}


Run: This function will change the context from the main thread to the first function of the ReadyQ

yield: This function will change the context from current function to next function in the ReadyQ and header to the ReadyQ will move to next function

*/

#include "q.h"


node* ReadyQ;
node* Curr_Thread;
int globalThread=1;

void start_thread(void (*function)(void))
{ 
	void *stack;
	TCB_t *item;
	stack = malloc(8192);
	item = (TCB_t *)malloc(sizeof(TCB_t));
	init_TCB(item, function, stack, 8192);
	item->threadid=globalThread;
	node *pushItem=NewItem(&item);
	AddQueue(&ReadyQ,&pushItem);
	globalThread++;
}

void run()
{      

	ucontext_t parent;    
	getcontext(&parent);
	Curr_Thread=ReadyQ;
	swapcontext(&parent, &(ReadyQ->context)); 
}

void yield() 
{       
	Curr_Thread=ReadyQ;
	RotateQueue(&ReadyQ);
	swapcontext(&(Curr_Thread->context),&(ReadyQ->context));
	
}


