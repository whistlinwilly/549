// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <process.h>



void serverLoop(void *);
void clientLoop(void);

Table * table;
Projector * projector;
int main()
{
	table = new Table();
	projector = new Projector();
	// create thread with arbitrary argument for the run function
    _beginthread(serverLoop, 0, (void*)12);

	clientLoop();
	return 0;
}


void serverLoop(void * arg) 
{ 
    while(true) 
    {
        table->update();
    }
}

void clientLoop()
{
    while(true)
    {
        //do game stuff
       //will later run client->update();
    }
}