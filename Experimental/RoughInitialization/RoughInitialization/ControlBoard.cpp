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
Point2f myPoints[] = {Point2f(550.0, 650.0), Point2f(710.0, 610.0), Point2f(750.0,580.0), Point2f(500.0, 700.0)};

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
Mat inverse;
float x1y1, x2y1, x3y1,x1y2,x2y2,x3y2,x1y3,x2y3,x3y3;
float ans1, ans2, ans3;
	for(int i = 0; i<4; i++){
		tProj.renderFrame(myPoints[i]);
			rect = tCam.extractPoint(cp);
				myMapping[i] = rect.center;
	}
	
	homey = getPerspectiveTransform(myPoints, myMapping);

	inverse = homey.inv();

	x1y1 = inverse.at<double>(0);
	x2y1 = inverse.at<double>(1);
	x3y1 = inverse.at<double>(2);
	x1y2 = inverse.at<double>(3);
	x2y2 = inverse.at<double>(4);
	x3y2 = inverse.at<double>(5);
	x1y3 = inverse.at<double>(6);
	x2y3 = inverse.at<double>(7);
	x3y3 = inverse.at<double>(8);

	waitKey();

	 ans1 = x1y1 * 324.0 + x2y1 * 240.0 + x3y1 * 1.0;
	 ans2 = x1y2 * 324.0 + x2y2 * 240.0 + x3y2 * 1.0;
	 ans3 = x1y3 * 324.0 + x2y3 * 240.0 + x3y3 * 1.0;

	 tProj.renderFrame(Point2f(ans1 / ans3, ans2 / ans3));

	return homey;
}