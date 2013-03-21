// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void serverLoop(void *);
void parseReceived(char *string);

Table * table;
char* recBuf = (char*)malloc(MAX_REC_BUF*sizeof(char));
char* sendBuf;
int main()
{
	table = new Table();
	while(1){
		if (table->network->acceptNewClient(table->client_id)){

		// create thread with arbitrary argument for the run function
			_beginthread(serverLoop, 0, (void*)12);
			table->client_id++;
		}
	}

	return 0;
}


void serverLoop(void * arg) 
{ 
    while(true) 
    {
		printf("Client connected, press any key to start sending\n");
		_getch();
		table->network->sendToAll("0", 5, 0);
		printf("Sent Message: INIT\n");
		table->network->receiveData(0, recBuf);
		parseReceived(recBuf);
		printf("Received Message: %s\n", recBuf);
		_getch();
		table->network->sendToAll("1", 5, 0);
		printf("Sent Message: INITCOORDS\n");
		table->network->receiveData(0, recBuf);
		parseReceived(recBuf);
		printf("Received Message: %s\n", recBuf);
		_getch();
		table->network->sendToAll("2", 5, 0);
		printf("Sent Message: OBJECT\n");
		table->network->receiveData(0, recBuf);
		parseReceived(recBuf);
		printf("Received Message: %s\n", recBuf);
		_getch();
		table->network->sendToAll("3", 5, 0);
		printf("Sent Message: STOP\n");
		table->network->receiveData(0, recBuf);
		parseReceived(recBuf);
		printf("Received Message: %s\n", recBuf);
		_getch();
		closesocket(table->network->ClientSocket);
		closesocket(table->network->ListenSocket);
        table->update();
    }
}

void parseReceived(char *string){
	int i = 0;
	char buffer[MAX_REC_BUF];
	while (((string[i] > 64 && string [i] < 91) || (string[i] > 96 && string[i] < 123)) && (i < MAX_REC_BUF)){
		buffer[i] = string[i];
		i++;
	}

	//cut off excess in string and add null terminator
	if (i < MAX_REC_BUF) string[i] = '\0';
	else printf("Receive Buffer is too small for incoming data!");
}