// Table.cpp : Defines the entry point for the console application.
//

#include "Initialization.h"


int main(int argc, char* argv[])
{
	Initialization* tableInit = new Initialization();

	tableInit->init(1);
	tableInit->connectToProjectors("10.0.1.187", 6881);
	tableInit->camRotation();
	tableInit->readInit1(0);
	tableInit->readInit2(0);
	tableInit->computeAndSendPerspectives(0);

	tableInit->mapper->clearGlobals();

	tableInit->readInit1(1);
	tableInit->readInit2(1);
	tableInit->computeAndSendPerspectives(1);

	tableInit->sn->sendToAll("3",5,0);
	tableInit->sn->receiveData(0,tableInit->recvbuf);
	tableInit->sn->sendToAll("3",5,1);
	tableInit->sn->receiveData(1,tableInit->recvbuf);

	return 0;
}

