#include "Initialization.h"
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>
#include <algorithm>

using namespace cv;
using namespace std;


Initialization::Initialization(){
}

void Initialization::init(int camNumber){
	tCam.initFastCam(camNumber);
	projectorsConnected = 0;
	recvbuf = (char*)malloc(MAX_REC_BUF*sizeof(char));
	tableBGsub = new BackgroundSubtractorMOG2(0, 245, true);
	mapper = new Mapper();
	mapper2 = new Mapper();
}

void Initialization::connectToProjectors(char* ip, int port){
	sn = new ServerNetwork(ip, port);

	//connects to all projectors
	while (projectorsConnected < NUM_PROJECTORS){
		sn->acceptNewClient(projectorsConnected);
		projectorsConnected++;
	}

	//sends init number to projectors to test connection
	sn->sendToAll("0",5,0);
	sn->receiveData(0,recvbuf);
	sn->sendToAll("0",5,1);
	sn->receiveData(1,recvbuf);
}

void Initialization::camRotation(void){
	int keyPressed;

	curWindow = "camRotation";
	namedWindow(curWindow, CV_WINDOW_AUTOSIZE);
	CameraPerspective test = tCam.tryRotation();
	imshow(curWindow, test.pic);
	keyPressed = waitKey();
	if (keyPressed == 121){
		test = tCam.findCorners();
		cp = test;
	}
	else if (keyPressed == 110)
		camRotation();
	else
		exit(0);
}

void Initialization::bgSubtraction(void){
	bgFrame = tCam.grabFrameWithPerspective(cp);
	imshow("FRAME GRABBED", bgFrame);
	tableBGsub->operator()(bgFrame, bgForeground, 0.001);
}

void Initialization::readInit1(int projNum){
	Mat bThresh, bCan;
	Mat twoCircles;

	frame = tCam.grabFrameWithPerspective(cp);
	tableBGsub->operator()(frame, foreground, 0.001);

	sn->sendToAll("1",5,projNum);
	sn->receiveData(projNum,recvbuf);
	waitKey(100);
	bgSubtraction();

	threshold(bgForeground,bThresh, 245.0, 255.0, THRESH_BINARY);
	Canny(bThresh, bCan, 100, 250, 5);

	//find the first patterns circles (tiny circles)
	if (projNum == 0){
		twoCircles = ImageManip::extractDoubleCircleData(mapper->cDatSmall, bCan, 20, 100, 20, bgFrame);
	}
	else {
		twoCircles = ImageManip::extractDoubleCircleData(mapper2->cDatSmall, bCan, 20, 100, 20, bgFrame);
	}
	imshow("CIRCLES", twoCircles);
	waitKey(100);

	
	
}

void Initialization::readInit2(int projNum){
	Mat bThresh, bCan;
	Mat twoCircles;
	

	sn->sendToAll("2",5,projNum);
	sn->receiveData(projNum,recvbuf);

	waitKey(100);
	bgSubtraction();

	imshow("BACKGROUND FRAME 2", bgFrame);
	imshow("BACKGROUND MODEL 2", bgForeground);
	//waitKey(100);

	threshold(bgForeground,bThresh, 245.0, 255.0, THRESH_BINARY);

	imshow("BACKGROUND THRESHOLD 2", bThresh);
	//waitKey(100);

	Canny(bThresh, bCan, 100, 250, 5);
	//waitKey(100);
	imshow("BACKGROUND CANNY 2", bCan);

	if (projNum == 0){
		twoCircles = ImageManip::extractDoubleCircleData(mapper->cDatBig, bCan, 50, 600, 20, bgFrame);
	}
	else {
		twoCircles = ImageManip::extractDoubleCircleData(mapper2->cDatBig, bCan, 50, 600, 20, bgFrame);
	}

	imshow("CIRCLES 2", twoCircles);
	waitKey(100);
}

void Initialization::computeAndSendPerspectives(int projNum){
	
	if (projNum == 0){
		mapper->computeProjectorData();
		mapper->computeUpVector();
		sn->sendToAll(mapper->coordString,strlen(mapper->coordString),projNum);
		sn->receiveData(projNum,recvbuf);
	}
	else {
		mapper2->computeProjectorData();
		mapper2->computeUpVector();
		sn->sendToAll(mapper2->coordString,strlen(mapper2->coordString),projNum);
		sn->receiveData(projNum,recvbuf);
	}
				

	sn->sendToAll("3",5,projNum);
	sn->receiveData(projNum,recvbuf);

	
	sn->sendToAll("0",5,projNum);
	sn->receiveData(projNum,recvbuf);
} 