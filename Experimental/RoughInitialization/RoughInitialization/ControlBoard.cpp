#include "ControlBoard.h"
#include "Projector.h"
#include "Camera.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>


using namespace std;
using namespace cv;

void projInit( int event, int x, int y, int flags, void* param );
void camRotationTest(void);
Mat getPointMapping(void);

Projector tProj;
Camera tCam;
String curWindow;
cameraPerspective cp;
Point2f myMapping[4];
Point2f myPoints[] = {Point2f(550.0, 550.0), Point2f(510.0, 510.0), Point2f(500.0,600.0), Point2f(575.0, 500.0)};

ControlBoard::ControlBoard(){
}

void ControlBoard::init(void){
	Mat myMat;
	RotatedRect myRect;
	cameraPerspective test;
	cameraPerspective nTest;
	curWindow= "projInit";
	namedWindow(curWindow, CV_WINDOW_AUTOSIZE);
	setMouseCallback(curWindow, projInit, (void*) NULL);
	imshow(curWindow, imread("projInit.jpg"));
	waitKey();
	myMat = getPointMapping();

}

void projInit( int event, int x, int y, int flags, void* param ){
	switch( event ){
		case CV_EVENT_LBUTTONUP:
			tProj.init();
			tProj.renderFrame(Point2f(0.0,0.0));
			camRotationTest();
			break;
	}
}


void camRotationTest(void){
	destroyWindow(curWindow);
	curWindow = "camRotation";
	namedWindow(curWindow, CV_WINDOW_AUTOSIZE);
	cameraPerspective myTest = tCam.tryRotation();
	imshow(curWindow, myTest.pic);
	int keyPressed = waitKey();
	if(keyPressed == 121){
		myTest = tCam.findCorners();
		cp = myTest;
		imshow(curWindow, myTest.pic);
	}
	else if(keyPressed == 110)
		camRotationTest();
	else
		exit(0);
}

Mat getPointMapping(){

RotatedRect rect;
Mat homey;
float x1y1, x2y1, x3y1,x1y2,x2y2,x3y2,x1y3,x2y3,x3y3;

	for(int i = 0; i<4; i++){
		tProj.renderFrame(myPoints[i]);
			rect = tCam.extractPoint(cp);
				myMapping[i] = rect.center;
	}
	
	homey = getPerspectiveTransform(myPoints,myMapping);

	x1y1 = homey.at<double>(0);
	x2y1 = homey.at<double>(1);
	x3y1 = homey.at<double>(2);
		x1y2 = homey.at<double>(3);
	x2y2 = homey.at<double>(4);
	x3y2 = homey.at<double>(5);
		x1y3 = homey.at<double>(6);
	x2y3 = homey.at<double>(7);
	x3y3 = homey.at<double>(8);

	waitKey();


	return homey;
}