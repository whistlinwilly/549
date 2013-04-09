// Table.cpp : Defines the entry point for the console application.
//

#include "Initialization.h"
#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Initialization* tableInit = new Initialization();
	tableInit->init(1);
	tableInit->connectToProjectors("10.0.1.2", 6881);
	tableInit->camRotation();
	tableInit->bgSubtraction();
	tableInit->readInit1();
	tableInit->bgSubtraction();
	tableInit->readInit2();
	tableInit->computeAndSendPerspectives();
	return 0;
}

