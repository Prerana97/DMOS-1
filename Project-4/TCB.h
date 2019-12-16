/*
Submission By

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID:1217164507

Structure of TCB_t is declared  with context, threadid, previou and next pointer address and given a alias name of TCB_t

init_TCB: It will initialize the TCB with context and will assign the stackspace and assign the stackpointer with the starting address of assigned stack space and finally make a contect with function pointer as mentioned in the project.

void init_TCB (TCB_t *tcb, void *function, void *stackP, int stack_size)

// arguments to init_TCB are

//   1. pointer to the function, to be executed

//   2. pointer to the thread stack

//   3. size of the stack

{

    memset(tcb, ’\0’, sizeof(TCB_t));       // wash, rinse

    getcontext(&tcb->context);              // have to get parent context, else snow forms on hell

    tcb->context.uc_stack.ss_sp = stackP;

    tcb->context.uc_stack.ss_size = (size_t) stack_size;

    makecontext(&tcb->context, function, 0);// context is now cooked

}


*/


#include <ucontext.h>
#include <string.h>

typedef struct TCB_t {
	ucontext_t context;
	int threadid;
	struct TCB_t *next;
	struct TCB_t *previous;
} TCB_t;


void init_TCB(TCB_t *tcb, void *functionPointer, void *stackPointer, int stackSize)
{
	memset(tcb, '\0', sizeof(TCB_t)); 
	getcontext(&tcb->context);
	tcb->context.uc_stack.ss_sp = stackPointer;
	tcb->context.uc_stack.ss_size = (size_t)stackSize; 
	makecontext(&tcb->context, functionPointer, 0); 
}

