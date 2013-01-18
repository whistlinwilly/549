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
Point2f myPoints[] = {Point2f(450.0, 450.0), Point2f(500.0, 500.0), Point2f(400.0,500.0), Point2f(500.0, 400.0)};

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

	for(int i = 0; i<4; i++){
		tProj.renderFrame(myPoints[i]);
			rect = tCam.extractPoint(cp);
				myMapping[i] = rect.center;
	}
	Mat homey;
	homey = getPerspectiveTransform(myPoints,myMapping);
		Point newCircles[2];
	Point myCircles[] = {Point(240,240), Point(480,480)};
	float testdata[] = {240.0,240.0,1.0};
Mat testPoint = Mat(3, 1, CV_32F, testdata).clone();
	
	testPoint = homey * testPoint;
	waitKey();
	return homey;
}