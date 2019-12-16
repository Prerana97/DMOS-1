/*
Submission By 

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID: 1217164507

*/

#include "msgs.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int port = 0; // port number assignment to servers & clients starting from 0
Semaphore_t *mutex;
static int servers = 0; // no of servers
static int clients = 0; // no of clients
int serverPorts[99]={0};// the server port number of a particular server thread is accessed from a globally accessible array
int clientPorts[99]={0};// the client port number of a particular client thread is accessed from a globally accessible array
//time_t t;

void server ()
{
	 
/*
When a server thread is initialized, it is assigned with a fixed port number which is stored in the serverPorts array. The server waits until it receives data from a client. Client sends its client number in the 0th index of message array (we derive client port number from the globally accessible array of clientPorts) so that the server will respond to the same port number. The server prints the data it receives from the client. It sends a reply back to the client on the client port number with a message which is the received message from client mutliplied by two.We are multiplying the message received from client by 2 except for element in zero index because it will help for the better understanding and validation of messages getting relayed i.e 0th index (of received message and sent message) will always have the client number from which data is received and to which data is sent respectively. 
*/		
	
	int portNo;
	int replyTo=0;
	int message[N]={0,0,0,0,0,0,0,0,0,0};
	int received[N];
	int serverNo;
	int count=0;
	P(&mutex);
		portNo=port++;
		serverPorts[servers]=portNo;
		serverNo=servers++;
	V(&mutex);
	while(1)
	{
		if(portNo>99)
		{
			printf("\nPorts are not available");
			break;
		}
		receive(portNo,received);
		count=0;
		while(count<N)
		{
			printf("\nServer :%d received data: %d from client %d",serverNo,received[count],received[0]);
			++count;
		}
		printf("\n");
		replyTo=clientPorts[received[0]];
		sleep(1);
		count=1;
		message[0]=received[0];
		while(count<N)
		{
			message[count]=received[count]*2;
			++count;
		}
		send(replyTo,message);
		printf("\n\t\t\t\t\tServer %d sent message successfully to client %d",serverNo,received[0]);
	}
}


void client ()
{
/*
The client fixes a server port initially by randomnly selecting one server port.The port of the client is kept in a globally accessible array clientPorts so that the server can reply to the particular client with the help of that client port number. A message which contains an array of integers (in the 0 th index client number will be given) is sent to the server on the initially selected serverport and sever will respond to client (based on the client number there in 0th index of message sent by client) with a message(which is just doubling the data here integers sent by client except at zero index as it will have client number it will be useful for validating the client number that is to see if server is replying to the correct client or not) and that received message from the server is printed. Server will wait till the client sends the message then only server can respond to the client.
*/
	int portNo;
	int clientNo;
	int serverPortNo=serverPorts[rand() % servers];
	int message[N]={1,1,1,1,1,1,1,1,1,1};
	int received[N];
	int count=0;
	P(&mutex);
		portNo=port++;
		clientPorts[clients]=portNo;
		clientNo=clients++;
	V(&mutex);
	while(1){
		if(portNo>99)
		{
			printf("\nPorts are not available");
			break;
		}
		message[0]=clientNo;
		send(serverPortNo,message);
		printf("\n\t\t\t\t\tClient %d sent message successfully to server %d",clientNo,serverPortNo);
		count=0;
		while(count<N)
		{
			++message[count];
			++count;
		}
		sleep(1);
		receive(portNo,received);
		count=0;
		while(count<N)
		{
			printf("\nClient :%d received data: %d from server %d",clientNo,received[count],serverPortNo);
			++count;
		}
		printf("\n");
	}	
	
}


int main(){
	ReadyQ = (node*) malloc(sizeof(node)); 
    	ReadyQ=newQueue();
	initPort();
	mutex = CreateSem(1);
	start_thread(server);
	start_thread(server);
        start_thread(client);
	start_thread(client);
	start_thread(client);
    
   	run();
	while(1){
		sleep(1);
	}
}
