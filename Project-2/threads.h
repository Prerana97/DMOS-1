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


void run()

{   // real code

    Curr_Thread = DelQueue(ReadyQ)

    ucontext_t parent;     // get a place to store the main context, for faking

    getcontext(&parent);   // magic sauce

    swapcontext(&parent, &(Curr_Thread->conext));  // start the first thread
}

void yield() // similar to run
{  TCB_t Prev_Thread;
   AddQueue(Ready_Q, Curr_Thread);
   Prev_Thread = Curr_Thread;
   Curr_Thread = DelQueue(ReadyQ);
   swap the context, from Prev_Thread to the thread pointed to Curr_Thread

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
	Curr_Thread=DelQueue(&ReadyQ); 
	swapcontext(&parent, &(Curr_Thread->context)); 
}

void yield() 
{       
	TCB_t* Prev_Thread;
	Prev_Thread=NewItem(&Curr_Thread);
	AddQueue(&ReadyQ,&Prev_Thread);
	Curr_Thread=DelQueue(&ReadyQ);
	swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));
	
}


