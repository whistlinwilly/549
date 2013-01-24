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
Point2f myOrigin[4];
Point2f myPoints[] = {Point2f(650.0, 410.0), Point2f(320.0, 200.0), Point2f(960.0, 200.0), Point2f(960.0, 600.0), Point2f(320.0, 600.0), Point2f(630.0, 300.0), Point2f(800.0, 420.0), Point2f(640.0, 500.0), Point2f(480.0, 390.0), Point2f(580.0,350.0),Point2f(700.0,350.0), Point2f(580.0,450.0), Point2f(700.0, 450.0)};

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
Mat transform;
Mat pic;
Mat pic2;
Mat pic3;
float x1y1, x2y1, x3y1,x1y2,x2y2,x3y2,x1y3,x2y3,x3y3;
float ans1, ans2, ans3, ans4, ans5, ans6, ans7, ans8, ans9, ans10, ans11, ans12, ans13, ans14, ans15;
float ans16, ans17, ans18, ans19, ans20, ans21, ans22, ans23, ans24, ans25, ans26, ans27;
int i=0;
int found = 0;
	while(i < sizeof(myPoints) && found < 4){
		tProj.renderFrame(myPoints[i]);
		rect = tCam.extractPoint(cp);
		if(rect.center.x > 0 && rect.center.y > 0){
			myOrigin[found] = myPoints[i];
			myMapping[found++] = rect.center;
		}
		i++;
	}
	
	homey = getPerspectiveTransform(myOrigin, myMapping);

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

//	double m[3][3] = {{x1y1, x2y1, x3y1}, {x1y2, x2y2, x3y2}, {x1y3, x2y3, x3y3}};
//	transform = cv::Mat(3, 3, CV_64F, m);

	//all of this crap should be done with matrix multiplication duh

	 ans1 = x1y1 * 324.0 + x2y1 * 240.0 + x3y1 * 1.0;
	 ans2 = x1y2 * 324.0 + x2y2 * 240.0 + x3y2 * 1.0;
	 ans3 = x1y3 * 324.0 + x2y3 * 240.0 + x3y3 * 1.0;

	 ans1 = ans1 / ans3;
	 ans2 = ans2 / ans3;

	 ans4 = x1y1 * 324.0 + x2y1 * 220.0 + x3y1 * 1.0;
	 ans5 = x1y2 * 324.0 + x2y2 * 220.0 + x3y2 * 1.0;
	 ans6 = x1y3 * 324.0 + x2y3 * 220.0 + x3y3 * 1.0;

	 ans4 = ans4 / ans6;
	 ans5 = ans5 / ans6;

	 ans7 = x1y1 * 324.0 + x2y1 * 260.0 + x3y1 * 1.0;
	 ans8 = x1y2 * 324.0 + x2y2 * 260.0 + x3y2 * 1.0;
	 ans9 = x1y3 * 324.0 + x2y3 * 260.0 + x3y3 * 1.0;

	 ans7 = ans7 / ans9;
	 ans8 = ans8 / ans9;

	 ans10 = x1y1 * 304.0 + x2y1 * 240.0 + x3y1 * 1.0;
	 ans11 = x1y2 * 304.0 + x2y2 * 240.0 + x3y2 * 1.0;
	 ans12 = x1y3 * 304.0 + x2y3 * 240.0 + x3y3 * 1.0;

	 ans10 = ans10 / ans12;
	 ans11 = ans11 / ans12;

	 ans13 = x1y1 * 344.0 + x2y1 * 240.0 + x3y1 * 1.0;
	 ans14 = x1y2 * 344.0 + x2y2 * 240.0 + x3y2 * 1.0;
	 ans15 = x1y3 * 344.0 + x2y3 * 240.0 + x3y3 * 1.0;	

	 ans13 = ans13 / ans15;
	 ans14 = ans14 / ans15;

	 ans16 = x1y1 * 150.0 + x2y1 * 240.0 + x3y1 * 1.0;
	 ans17 = x1y2 * 150.0 + x2y2 * 240.0 + x3y2 * 1.0;
	 ans18 = x1y3 * 150.0 + x2y3 * 240.0 + x3y3 * 1.0;	

	 ans16 = ans16 / ans18;
	 ans17 = ans17 / ans18;

	 ans19 = x1y1 * 550.0 + x2y1 * 240.0 + x3y1 * 1.0;
	 ans20 = x1y2 * 550.0 + x2y2 * 240.0 + x3y2 * 1.0;
	 ans21 = x1y3 * 550.0 + x2y3 * 240.0 + x3y3 * 1.0;	

	 ans19 = ans19 / ans21;
	 ans20 = ans20 / ans21;

	 ans22 = x1y1 * 324.0 + x2y1 * 100.0 + x3y1 * 1.0;
	 ans23 = x1y2 * 324.0 + x2y2 * 100.0 + x3y2 * 1.0;
	 ans24 = x1y3 * 324.0 + x2y3 * 100.0 + x3y3 * 1.0;	

	 ans22 = ans22 / ans24;
	 ans23 = ans23 / ans24;

	 ans25 = x1y1 * 324.0 + x2y1 * 380.0 + x3y1 * 1.0;
	 ans26 = x1y2 * 324.0 + x2y2 * 380.0 + x3y2 * 1.0;
	 ans27 = x1y3 * 324.0 + x2y3 * 380.0 + x3y3 * 1.0;	

	 ans25 = ans25 / ans27;
	 ans26 = ans26 / ans27;



	 tProj.renderStar(Point2f(ans1, ans2), Point2f(ans4, ans5), Point2f(ans7, ans8),Point2f(ans10, ans11), Point2f(ans13, ans14), Point2f(ans16, ans17), Point2f(ans19,ans20),Point2f(ans22, ans23), Point2f(ans25,ans26));
	 
	 pic = imread("projInit.jpg");
	 Mat rot_mat = getRotationMatrix2D(cp.center,cp.angle,1);
	warpAffine(pic, pic2, rot_mat, pic.size(), INTER_CUBIC);
	 warpPerspective(pic2, pic3, inverse, pic.size());

	 imshow("camRotation", pic);
	 waitKey();

	 imshow("camRotation", pic2);
	 waitKey();

	 imshow("camRotation", pic3);
	 waitKey();


	return homey;
}