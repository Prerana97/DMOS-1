// (c) Partha Dasgupta 2009
// permission to use and distribute granted.
// Snehitha Rednam  --  1217164507 & Tejaswi Paruchuri -- 1213268054

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


pthread_t start_thread(void *func, int *arg)
{
   pthread_t thread_id;
   int rc;
   printf("In main: creating thread\n");
   rc = pthread_create(&thread_id, NULL, func, arg);
   if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
    }
    return(thread_id);
}

typedef struct semaphore_t {
     pthread_mutex_t mutex;
     pthread_cond_t cond;
     int count;
} semaphore_t;

void init_sem(semaphore_t *s, int i)
{   
    s->count = i;
    pthread_mutex_init(&(s->mutex), NULL);
    pthread_cond_init(&(s->cond), NULL);
}


/*
 * The P routine decrements the semaphore, and if the value is less than
 * zero then blocks the process 
 */
void P(semaphore_t *sem)
{   
    pthread_mutex_lock (&(sem->mutex)); 
    sem->count--;
    if (sem->count < 0) pthread_cond_wait(&(sem->cond), &(sem->mutex));
    pthread_mutex_unlock (&(sem->mutex)); 
}	


/*
 * The V routine increments the semaphore, and if the value is 0 or
 * negative, wakes up a process and yields
 */

void V(semaphore_t * sem)
{   
    pthread_mutex_lock (&(sem->mutex)); 
    sem->count++;
    if (sem->count <= 0) {
	pthread_cond_signal(&(sem->cond));
    }
    pthread_mutex_unlock (&(sem->mutex)); 
    pthread_yield();
}

semaphore_t common,thread1,thread2,thread3;


void function_1(int *arg)
{
    while (1){
        P(&thread1);
        //printf("Beginning of CS: func 1\n");
	*arg=*arg+1;
        //printf("End of CCS: func 1..\n");
	P(&common);
    }
}    

void function_2(int *arg)
{
    while (1){
        P(&thread2);
        //printf("Beginning of CS: func 2\n");
	*arg=*arg+1;
        //printf("End of CCS: func 2..\n");
	P(&common);
    }
}    

void function_3(int *arg)
{
    while (1){
        P(&thread3);
       //printf("Beginning of CS: func 3\n");
	*arg=*arg+1;
        //printf("End of CCS: func 3..\n");
	P(&common);
    }
}    


int main()
{  
    int a[3]={0,0,0};
    init_sem(&common,3);
    init_sem(&thread1, 1);
    init_sem(&thread2, 1);
    init_sem(&thread3, 1);
    start_thread(function_1, &a[0]);
    start_thread(function_2, &a[1]);
    start_thread(function_3, &a[2]);
    while(1)
    {
	   if(common.count==0){
	   	printf("\n%d %d %d\n",a[0],a[1],a[2]);
           	V(&thread1); V(&thread2); V(&thread3);
	   	V(&common); V(&common); V(&common);
           }
           sleep(1);
   }

    return 0;
}



