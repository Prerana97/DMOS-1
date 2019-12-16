
/*
Submission By 

Name: Tejaswi Paruchuri
ASU ID: 1213268054

Name: Rednam Snehitha
ASU ID: 1217164507

Execute it as ./a.out 5 f1 f2 f3 f4 f5
*/
#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include "msgs.h"

#define SOURCE_PORT 99

int client_port = 1;
char * listFileNames[100];

void server() 
{

/*
Client can send below 3 types of message status to server.
	message status 0- to send fileName and asking server to create the file at their end if everything is fine.
		-->If server responds with 'BUSY'(if server is already processing 3 files) then cleint will wait till the server is free
		-->If server responds with 'filename' (filename length is more than 15 characters) then client will exit from that file transfer
		-->If server responds 'DONE' (everything is fine at server end and it has created file at it's end ) then client will try to send another file name
	message status 1- to send 32 characters content of the file to server each time
		-->If server responds with 'filesize' (every time server writes one character to file and checks if file size is more than 1MB. If file size is more than 1MB it will respond with filesize) then client will understand it has already sent more than 1MB of data to server already and exit from data transfer.
		-->If server responds with 'DONE' (server has successfully written 32 characters of data sent by client) client will further send content of file till everything is sent
	message status 2- client will server know that file transfer is completed.
		-->Server responds with 'COMPLETE' (saying it has received all the contents of file from client) then client will start sending another file if any files are left
*/
	int msg[10];
  	int reply_port;
  	int requestCount = 0;
  	char file_map[100][22];
	char buffer[32];
 	int i, j;

  	while (1) 
	{
    		receive(SOURCE_PORT, msg);
   		reply_port = msg[0];
    		sleep(1);
    		if (msg[1] == 0) 
		{	
      			//Message status received from client is 0--server receiving fileName
      			if (requestCount >= 3) 
			{
				//Server is already busy with 3 file transfers form client
       				msg[0] = 'B';
        			msg[1] = 'U';
        			msg[2] = 'S';
        			msg[3] = 'Y';
        			msg[4] = '\0';
        			printf("\nServer: There are 3 files being served, hence busy");
        			send(reply_port, msg);

      			} 
			else 
			{
        			requestCount++;
        			char fileName[22];
        			memcpy(fileName, msg + 2, 32);
        			if (strlen(fileName) > 15) 
				{
					//filename is more than 15 characters hence sending message to client that it hasn't created frile at it's end and asking client to stop file transfer
          				msg[0] = 'f';
          				msg[1] = 'i';
          				msg[2] = 'l';
          				msg[3] = 'e';
          				msg[4] = 'n';
          				msg[5] = 'a';
          				msg[6] = 'm';
          				msg[7] = 'e';
          				msg[8] = '\0';
          				printf("\nServer: You are trying to send a file with filename more than 15 characters, decrease the file name size\n");
          				requestCount--;
          				send(reply_port, msg);
        			} 
				else 
				{
	  				//Everything is fine and server creates file at its end and responds with the same to client
          				strcat(fileName, ".server");
          				printf("\nServer creating file %s \n", fileName);
          				FILE * fp = fopen(fileName, "w");
          				fclose(fp);
          				strcpy(file_map[reply_port], fileName);
          				msg[0] = 'D';
          				msg[1] = 'O';
          				msg[2] = 'N';
          				msg[3] = 'E';
          				msg[4] = '\0';
          				send(reply_port, msg);
        			}
      			}
    		} 
		else if (msg[1] == 1) 
		{
    			//Message status received -1, Receiving the file contents from the client
      			int sizeFlag = 0, i;
      			char fileName[22];
      			strncpy(fileName, file_map[reply_port], sizeof(file_map[reply_port]));
      			FILE * fp = fopen(fileName, "a");
      			printf("\nFile: %s data receive in progress at Server",fileName);
      			memcpy(buffer, msg + 2, 32);
      			for (i = 0; i < strlen(buffer); i++) 
			{
				// writing one character at once to file and every time checking file size to make sure no more than 1MB of data is written in the server file
        			int size = ftell(fp);
        			if (size >= 1000) 
				{
	  				//Server already received 1MB of data in file hence asking client to stop further
          				sizeFlag = 1;
          				break;
        			} 
				else 
				{
					//otherwise write each character to the file in server
          				fprintf(fp, "%c", buffer[i]);
       		 		}
      			}
      			fclose(fp);
      			if (sizeFlag == 1) 
			{
				//Letting client know that server has already received more than 1MB of data by sending message
        			msg[0] = 'f';
        			msg[1] = 'i';
        			msg[2] = 'l';
        			msg[3] = 'e';
        			msg[4] = 's';
        			msg[5] = 'i';
        			msg[6] = 'z';
        			msg[7] = 'e';
        			msg[8] = '\0';
        			printf("\nFile size more than 1 MB hence stop\n");
				//requestCount--;

      			} 
			else 
			{
				//Otherwise sending message to client that it has copied data to file whatever client has send and asking client to send data further to sever of that particular file
        			msg[0] = 'D';
        			msg[1] = 'O';
        			msg[2] = 'N';
        			msg[3] = 'E';
        			msg[4] = '\0';
      			}
      			send(reply_port, msg);

    		} 
		else if (msg[1] == 2) 
		{
    			//Message status received -2, When the file transfer is complete from the client side	
      			char fileName[22];
      			strncpy(fileName, file_map[reply_port], sizeof(file_map[reply_port]));
      			printf("\nFile transfer completed from Client hence server is closing %s file -----\n", fileName);
      			requestCount--;
      			//Server letting client know that it has completely received the contents of the file.
      			msg[0] = 'C';
      			msg[1] = 'O';
      			msg[2] = 'M';
      			msg[3] = 'P';
      			msg[4] = 'L';
      			msg[5] = 'E';
      			msg[6] = 'T';
      			msg[7] = 'E';
      			msg[8] = '\0';
      			send(reply_port, msg);
    		}
  	}
}

void client() 
{

/*
Client can send below 3 types of messages to server.
	0- to send fileName and asking server to create the file at their end if everything is fine.
		-->If server responds with 'BUSY'(if server is already processing 3 files) then cleint will wait till the server is free
		-->If server responds with 'filename' (filename length is more than 15 characters) then client will exit from that file transfer
		-->If server responds 'DONE' (everything is fine at server end and it has created file at it's end ) then client will try to send another file name
	1- to send 32 characters content of the file to server each time
		-->If server responds with 'filesize' (every time server writes one character to file and checks if file size is more than 1MB. If it is so it will respond with filesize) then client will understand it has already sent more than 1MB of data to server already and exit from data transfer.
		-->If server responds with 'DONE' (server has successfully written 32 characters of data sent by client) client will further send content of file till everything is sent
	2- client will server know that file transfer is completed.
		-->Server responds with 'COMPLETE' (saying it has received all the contents of file from client) then client will start sending another file if any files are left
*/
 	char fileName[100];
  	int index;
  	memcpy(fileName, listFileNames[client_port - 1], 32); //copying the filename to fileName variable
  	int send_msg[10]; //message sent to server
  	int recv_msg[10]; //message received from server
  	int reply_port; //maintains client port number to which server has to respond
  	char buffer[32]; //char array to maintain contents of file to be transferred to sever
  	if (client_port == SOURCE_PORT - 1) 
	{
    		client_port = 1;
  	}
  	reply_port = client_port++;
  	int i;
  	send_msg[0] = reply_port;
  	send_msg[1] = 0; // Sending the filename to the server
  	memcpy(send_msg + 2, fileName, 32); //copying the fileName to message being sent to server
  	while (1) 
	{	
    		send(SOURCE_PORT, send_msg);
    		receive(reply_port, recv_msg); //reply from server is stored in recv_msg
    		char status[10];
    		for (i = 0; i < 10; i++) 
		{
      			if (recv_msg[i] != '\0') 
			{
        			status[i] = recv_msg[i];
      			} 
			else 
			{
        			status[i] = '\0';
        			break;
      			}

    		}
   		//client will respond based on the message received from sever as mentioned in comments above
    		if (strcmp(status, "filename") == 0) 
		{
      			printf("\n\t\t\tServer responded with filename :FileName greater than the permitted limit hence exiting %s file transfer.\n",fileName);
      			while (1) 
			{
        			yield();
      			}
    		}
    		if (strcmp(status, "DONE") != 0) 
		{
      			printf("\n\t\t\tServer responded with BUSY: Server is already busy with 3 files hence waiting for file %s transfer\n", fileName);
      			yield();
    		} 
		else 
		{
      			break;
    		}
  	}
	
  	i = 0;
  	FILE * fp = fopen(fileName, "r");
  	// client sending the contents of the file to the server
  	send_msg[0] = reply_port;
  	send_msg[1] = 1;
  	printf("\n\t\t\t%s file transfer started from client", fileName);
  	while (fgets(buffer, 32, fp)) 
	{
    		memcpy(send_msg + 2, buffer, 32);
    		printf("\n\t\t\tFile : %s file transfer in progress in client",fileName);
    		send(SOURCE_PORT, send_msg);
    		receive(reply_port, recv_msg);
    		char statusString[10];
    		for (i = 0; i < 10; i++) 	
		{
      			if (recv_msg[i] != '\0') 
			{
        			statusString[i] = recv_msg[i];
      			} 
			else 
			{
        			statusString[i] = '\0';
        			break;
      			}

    		}
    		if (strcmp(statusString, "filesize") == 0) 
		{
      			printf("\n\t\t\tServer responded with filesize : The file has size >1Mb. Further writing is blocked\n");
      			fclose(fp);
      			while (1) 
			{
        			yield();
      			}
    		}
  	}
  	printf("\n\t\t\t%s The file transfer is completed from client", fileName);
  	//When the client has sent all the required content of file, client is letting server know by sending message status as 2.
  	send_msg[1] = 2;
  	send_msg[2] = '\0';
  	send(SOURCE_PORT, send_msg);
  	receive(reply_port, recv_msg);
  	fclose(fp);
  	while (1) 
	{
    		yield();
  	}	

}

int main(int argc, char * argv[]) {
/*Creating one server and the clients. Number of clients depends on the number of files to be tranfered. For x files there will be x clients*/
 	ReadyQ = newQueue();
  	initPort();
  	start_thread(server);
  	int numberOfClients = atoi(argv[1]);
  	int i;
  	for (i = 0; i < numberOfClients; i++) 
	{
    		listFileNames[i] = (char * ) malloc(strlen(argv[2 + i]) * sizeof(char));
    		memcpy(listFileNames[i], argv[2 + i], strlen(argv[2 + i]) * sizeof(char));
    		start_thread(client);
  	}
  	run();
}
