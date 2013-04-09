#include "ControlBoard.h"
#include "Projector.h"
#include "Camera.h"
#include "ServerNetwork.h"
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>
#include <algorithm>


#define _USE_MATH_DEFINES

#include <math.h>


using namespace std;
using namespace cv;

ServerNetwork* sn;

void projInit( int event, int x, int y, int flags, void* param );
void camRotationTest(void);
Mat getPerspectiveMapping(void);
float myDistanceFromTable;
float myIncidence;
float myCenterX;
float myCenterY;
float myTwist;
float myRot;
static unsigned int projectorsConnected;

static char* recvbuf = (char*)malloc(MAX_REC_BUF*sizeof(char));

bool hasTrainingImage = false;
std::vector<cv::Point> train;

float h0, h1, h2, h3, h4, h5, h6, h0Er, h1Er, h2Er, h3Er, h4Er, h5Er, h6Er = 0.0;
int numH = 0;

Projector tProj;
BackgroundSubtractorMOG2* tableBGsub;
Camera tCam;
String curWindow;
cameraPerspective cp;
Point2f myMapping[4];
Point2f myOrigin[4];
Point2f myPoints[] = {Point2f(650.0, 410.0), Point2f(320.0, 200.0), Point2f(960.0, 200.0), Point2f(960.0, 600.0), Point2f(320.0, 600.0), Point2f(630.0, 300.0), Point2f(800.0, 420.0), Point2f(640.0, 500.0), Point2f(480.0, 390.0), Point2f(580.0,350.0),Point2f(700.0,350.0), Point2f(580.0,450.0), Point2f(700.0, 450.0)};
	Mat fixedPts;

ControlBoard::ControlBoard(){
}

void ControlBoard::init(void){

		
	projectorsConnected = 0;


	Mat myMat;

		sn = new ServerNetwork();

	while(projectorsConnected < NUM_PROJECTORS){
		sn->acceptNewClient(projectorsConnected);
		projectorsConnected++;
	}

	sn->sendToAll("0",5,0);
	sn->receiveData(0,recvbuf);

	//FROM MY TABLE INIT
	RotatedRect myRect;
	cameraPerspective test;
	cameraPerspective nTest;
	curWindow= "projInit";
	namedWindow(curWindow, CV_WINDOW_AUTOSIZE);
	setMouseCallback(curWindow, projInit, (void*) NULL);
	imshow(curWindow, imread("projInit.jpg"));
	waitKey();

	Mat newFrame = tCam.grabFrameWithPerspective(cp);
	Mat trans, foreground;
	tableBGsub = new BackgroundSubtractorMOG2(0,235,true);
	tableBGsub->operator()(newFrame,foreground,0.001);

	myMat = getPerspectiveMapping();
	namedWindow("ROI",WINDOW_AUTOSIZE);
	runObjectTracking();

}







void projInit( int event, int x, int y, int flags, void* param ){
	switch( event ){
		case CV_EVENT_LBUTTONUP:
			tCam.initFastCam();
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


float xToInches(float pixels){
	return ((pixels/TABLE_X)*TABLE_WIDTH);
}

float yToInches(float pixels){
	return ((pixels/TABLE_Y)*TABLE_WIDTH);
}

float toRads(float degrees){
	return ((degrees/360.0)*M_PI);
}

float toDegrees(float radians){
	return ((radians/M_PI)*360.0);
}

Mat getPerspectiveMapping(void){

	//OBJECT TRACKING VARIABLES
	Mat background;
	Mat foreground;
	//END VARS

	cameraPerspective test;
	Mat bThresh, bCan, grey;
	vector<Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > contour;
	RotatedRect myRect;

	circleData* cdat = new circleData;
	circleData* cdatTiny = new circleData;

	float bigWidth;
	float bigHeight;
	float rotation = 0;
	float temp;
	bool inCenter = true;
	float incidentAngle = 0;
	float curX = 0;
	float curY = 0;
	float x1y1, x2y1, x3y1,x1y2,x2y2,x3y2,x1y3,x2y3,x3y3;
	float ans1, ans2, ans3;
	float twist = 0.0;
	float bigTrueX;
	float bigTrueY;
	float smallTrueX;
	float smallTrueY;
	float eyeX;
	float eyeY;
	float eyeZ;
	float upX;
	float upY;
	float upZ;
	float focusX;
	float focusY;
	float focusZ;
	float bottomDilation;
	float distance = DEFAULT_DISTANCE;

	//tProj.renderInitPattern();

	Mat bgFrame, bgThresh, bgForeground;

	bgFrame = tCam.grabFrameWithPerspective(cp);
	tableBGsub->operator()(bgFrame,bgForeground,0.001);

	sn->sendToAll("1",5,0);
	sn->receiveData(0,recvbuf);

	waitKey(100);

	bgFrame = tCam.grabFrameWithPerspective(cp);

	imshow("FRAME GRABBED", bgFrame);
	waitKey();

	tableBGsub->operator()(bgFrame,bgForeground,0.001);

	//Mat back;

	//tableBGsub->getBackgroundImage(back);

	imshow("BACKGROUND MODEL", bgForeground);
	waitKey();

	threshold(bgForeground, bThresh, 245.0, 255.0,THRESH_BINARY);

	imshow("BACKGROUND THRESHOLD", bThresh);
	waitKey();

	Canny(bThresh, bCan, 100, 250, 5);

	imshow("BACKGROUND CANNY", bCan);
	waitKey();

	//find the tiny circles
	Mat twoCircles = extractDoubleCircleData(cdatTiny, bCan, 20, 100, 20, bgFrame);
	imshow("CIRCLES", twoCircles); 

	if (abs(cdatTiny->c1x) - abs(cdatTiny->c2x) > 1 || abs(cdatTiny->c1y) - abs(cdatTiny->c2y) > 1){
		bigTrueX = cdatTiny->c2x;
		bigTrueY = cdatTiny->c2y;
		smallTrueX = cdatTiny->c1x;
		smallTrueY = cdatTiny->c1y;
	}
	else{ 
		bigTrueX = cdatTiny->c1x;
		bigTrueY = cdatTiny->c1y;
		smallTrueX = cdatTiny->c2x;
		smallTrueY = cdatTiny->c2y;
	}

	waitKey();

	sn->sendToAll("2",5,0);
	sn->receiveData(0,recvbuf);

//	myServer->sendString("0",1);
	
//	myServer->confirm();

 	waitKey(100);

	bgFrame = tCam.grabFrameWithPerspective(cp);

	imshow("FRAME GRABBED", bgFrame);
	waitKey();

	tableBGsub->operator()(bgFrame,bgForeground,0.001);

	//Mat back;

	//tableBGsub->getBackgroundImage(back);

	imshow("BACKGROUND MODEL", bgForeground);
	waitKey();

	threshold(bgForeground, bThresh, 245.0, 255.0,THRESH_BINARY);

	imshow("BACKGROUND THRESHOLD", bThresh);
	waitKey();

	Canny(bThresh, bCan, 100, 250, 5);

	imshow("BACKGROUND CANNY", bCan);
	waitKey();

	twoCircles = extractDoubleCircleData(cdat, bCan, 100, 600, 20, bgFrame);
	imshow("CIRCLES 2", twoCircles); 
	waitKey();

	//CHECK RADIANS/DEGREES AND PIXELS/INCHES 
	//SET bottomDilation
	if(cdat->c1w > cdat->c2w){
		distance = cdat->c1w*DEFAULT_DISTANCE;
		//rotation may be wrong
		rotation = cdat->c1r;
		incidentAngle = acos(cdat->c1w/(2*bottomDilation));
	}
	else{
		distance = cdat->c2w*DEFAULT_DISTANCE;
		//rotation may be wrong
		rotation = cdat->c2r;
		incidentAngle = acos(cdat->c2w/(2*bottomDilation));
	}

	eyeX = distance*cos(90-incidentAngle)/(sqrt(1- pow(tan(rotation), 2))) + bigTrueX;
	eyeY = tan(rotation)*distance*(cos(90-incidentAngle)/sqrt(1-pow(tan(rotation), 2))) + bigTrueY;
	eyeZ = distance*sin(90-incidentAngle);
	twist = atan((bigTrueY-smallTrueY)/(bigTrueX-smallTrueX)) - rotation;

	stringstream ss (stringstream::in | stringstream::out);
		
	ss << eyeX;

	std::string s = ss.str();

	s.append(",");

	ss.str("");
	ss.clear();
	
	ss << eyeY;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << eyeZ;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << camCenterX;

	s.append(ss.str());
	s.append(",");

	ss.str("");
	ss.clear();

	ss << camCenterY;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	twist = 0;
	ss << twist;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << upX;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << upY;

	s.append(ss.str());

	s.append(",");

		ss.str("");
	ss.clear();

	ss << upZ;

	s.append(ss.str());

	s.append(",");

	char *cstr = new char[s.length() + 1];
	strcpy(cstr, s.c_str());

	sn->sendToAll(cstr,s.length(),0);
	sn->receiveData(0,recvbuf);

	sn->sendToAll("3",5,0);

	findContours( bCan, contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );

	return Mat();

}