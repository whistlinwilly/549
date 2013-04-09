#include "Initialization.h"
#include "ImageManip.h"
#include "ServerNetwork.h"
#include "Mapper.h"
#include "Camera.h"
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
	cDatSmall = new circleData;
	cDatBig = new circleData;
	tableBGsub = new BackgroundSubtractorMOG2(0, 235, true);
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
	waitKey();
	tableBGsub->operator()(bgFrame, bgForeground, 0.001);
}

void Initialization::readInit1(void){
	Mat bThresh, bCan;
	Mat twoCircles;

	sn->sendToAll("1",5,0);
	sn->receiveData(0,recvbuf);

	waitKey(100);

	imshow("BACKGROUND MODEL", bgForeground);
	waitKey();

	threshold(bgForeground,bThresh, 245.0, 255.0, THRESH_BINARY);

	imshow("BACKGROUND THRESHOLD", bThresh);
	waitKey();

	Canny(bThresh, bCan, 100, 250, 5);

	imshow("BACKGROUND CANNY", bCan);
	waitKey();

	//find the first patterns circles (tiny circles)
	twoCircles = ImageManip.extractDoubleCircleData(cDatSmall, bCan, 20, 100, 20, bgFrame);
	imshow("CIRCLES", twoCircles);

	waitKey();

}

void Initialization::readInit2(void){
	Mat bThresh, bCan;
	Mat twoCircles;

	sn->sendToAll("2",5,0);
	sn->receiveData(0,recvbuf);

	waitKey(100);

	imshow("BACKGROUND MODEL", bgForeground);
	waitKey();

	threshold(bgForeground,bThresh, 245.0, 255.0, THRESH_BINARY);

	imshow("BACKGROUND THRESHOLD", bThresh);
	waitKey();

	Canny(bThresh, bCan, 100, 250, 5);

	imshow("BACKGROUND CANNY", bCan);
	waitKey();

	twoCircles = ImageManip.extractDoubleCircleData(cDatBig, bCan, 100, 600, 20, bgFrame);
	imshow("CIRCLES 2", twoCircles);
	waitKey();
}

void computeAndSendPersepctives(void){
	
}