/*
Submission By

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID:1217164507

This file has four functions with infinite loops and similar structure 

Created global variable which will be used by all the functions that will be called by start_thread function
Each function has count local variable as well
In each function global and count variables are incremented by one which means global has to be commonly incremented by all the functions where as local will be incremented only with respect to that function.

Every function has yield which will context to function in the first node of the queue

*/


#include<unistd.h>
#include "threads.h"

int global = 0;

void function1() {
	int local = 0;
	while(1) {
		printf("ThreadID: %d\tLocal: %d\tGlobal: %d\t in function1\n",Curr_Thread->threadid, local, global);
		global++;
		local++;
		sleep(1);
		yield();
		printf("\nAfter yield\n");
		yield();
	}

}

void function2() {
	int local = 0;

	while(1) {
		printf("ThreadID: %d\tLocal: %d\tGlobal: %d\t in function2\n",Curr_Thread->threadid, local, global);
		global++;
		local++;
		sleep(1);
		yield();
	}
}

void function3() {
	int local = 0;
	while(1){
		printf("ThreadID: %d\tLocal: %d\tGlobal: %d\t in function3\n",Curr_Thread->threadid, local, global);
		global++;
		local++;
		sleep(1);
		yield();
	}
}

void function4() {
	int local = 0;
	while(1){
		printf("ThreadID: %d\tLocal: %d\tGlobal: %d\t in function4\n",Curr_Thread->threadid, local, global);
		global++;
		local++;
		sleep(1);
		yield();
	}
}

int main() {

	ReadyQ = (node*) malloc(sizeof(node)); 
	ReadyQ=newQueue(); //New queue is initated
	start_thread(function1);
	start_thread(function2);
	start_thread(function3);
	start_thread(function4);
	run();
	return 0;
}
