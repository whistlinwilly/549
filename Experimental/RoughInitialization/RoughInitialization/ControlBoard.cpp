#include "ControlBoard.h"
#include "Projector.h"
#include "Camera.h"


using namespace std;
using namespace cv;

void projInit( int event, int x, int y, int flags, void* param );
void camRotationTest(void);
void getPointMapping(void);

Projector tProj;
Camera tCam;
String curWindow;
cameraPerspective cp;
Point2f myMapping[10][10];

ControlBoard::ControlBoard(){
}

void ControlBoard::init(void){

	RotatedRect myRect;
	cameraPerspective test;
	cameraPerspective nTest;
	curWindow= "projInit";
	namedWindow(curWindow, CV_WINDOW_AUTOSIZE);
	setMouseCallback(curWindow, projInit, (void*) NULL);
	imshow(curWindow, imread("projInit.jpg"));
	waitKey();
	getPointMapping();


}

void projInit( int event, int x, int y, int flags, void* param ){
	switch( event ){
		case CV_EVENT_LBUTTONUP:
			tProj.init();
			tProj.renderFrame(0,0);
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

void getPointMapping(){

RotatedRect rect;

	for(int i = 4; i<7; i++)
		for(int j = 2; j < 8; j++){
			tProj.renderFrame(300 + i * 50, 200 + j * 50);
			rect = tCam.extractPoint(cp);
			myMapping[i][j] = rect.center;
		}

waitKey();
}