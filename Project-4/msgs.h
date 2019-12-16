/*
Submission By 

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID: 1217164507

The mutex semaphore in the port is for synchronizing the modification of the message.
The full semaphore ensures in making the thread using receive() on the port wait until a send() from a different thread increments the full semaphore of the same port.
The empty semaphore helps in synchronizing the send() of a thread using port by checking if there is a thread which is free to receive() on the same port.

*/

#include <stdlib.h>
#include "sem.h"

#define N 10


struct port{
	Semaphore_t* mutex;
	Semaphore_t* full;
	Semaphore_t* empty;
	int in;
	int out;
	int messages[N][N];
}p[100];



void initPort(){
	int i;
	for(i = 0; i<100; i++){
		
		p[i].mutex = CreateSem(1);
		p[i].full = CreateSem(0);
		p[i].empty = CreateSem(N);
		p[i].in = 0;
		p[i].out = 0;
	}
}


int send(int portNo, int message[]){ 
	P(&p[portNo].empty);
          P(&p[portNo].mutex);
          memcpy(p[portNo].messages[p[portNo].in], message, N*sizeof(int));
		  p[portNo].in = (p[portNo].in+1) % N;
          V(&p[portNo].mutex);
	V(&p[portNo].full);
	return 0;
}



int receive(int portNo, int message[]){ 
	P(&p[portNo].full);
          P(&p[portNo].mutex);
		  memcpy(message, p[portNo].messages[p[portNo].out], N*sizeof(int));
          p[portNo].out = (p[portNo].out+1) % N;
          V(&p[portNo].mutex);
	V(&p[portNo].empty);
	return 0;
}

