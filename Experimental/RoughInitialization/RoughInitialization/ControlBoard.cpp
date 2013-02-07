#include "ControlBoard.h"
#include "Projector.h"
#include "Camera.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>


#define _USE_MATH_DEFINES

#include <math.h>


using namespace std;
using namespace cv;

void projInit( int event, int x, int y, int flags, void* param );
void camRotationTest(void);
Mat getPointMapping(void);
Mat getPerspectiveMapping(void);

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
	myMat = getPerspectiveMapping();
	//myMat = getPointMapping();

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

Mat getPerspectiveMapping(void){
	cameraPerspective test;
	Mat bThresh, bCan, grey;
	vector<Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > contour;
	RotatedRect myRect;
	Mat fixedPts;

	float width = 0;
	float height = 0;
	float num = 0;
	float rot = 0;
	float centerX = 0;
	float centerY = 0;
	float diffX = 0;
	float diffY = 0;
	float temp;
	bool inCenter = true;
	float incidentAngle = 0;
	float curX = 0;
	float curY = 0;
	float x1y1, x2y1, x3y1,x1y2,x2y2,x3y2,x1y3,x2y3,x3y3;
	float ans1, ans2, ans3;
	
float distanceFromTable = DEFAULT_DISTANCE;

	tProj.renderInitPattern();

	test = tCam.getBackground(cp);

	//Works for brighter projector with multicolored test pattern
	IplImage rgb = test.background;


	IplImage* r = cvCreateImage( cvGetSize(&rgb), rgb.depth,1 );
	IplImage* g = cvCreateImage( cvGetSize(&rgb), rgb.depth,1 );
	IplImage* b = cvCreateImage( cvGetSize(&rgb), rgb.depth,1 );

	cvSplit(&rgb,b,g,r,NULL);

	Mat red = r;
	Mat blue = b;
	Mat green = g;

	//cvtColor(test.background, grey, CV_RGB2GRAY);

	threshold(blue, bThresh, 182.0, 255.0,THRESH_BINARY);

	imshow("circle", bThresh);

	waitKey();

	Canny(bThresh, bCan, 100, 250, 3);

	imshow("circle", bCan);

	waitKey();

	findContours( bCan, contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );

	for(int i=0; i<contour.size(); i++){
		if(contour[i].size() > 200){
			myRect = fitEllipse(contour[i]);
		if(myRect.size.area() > 1000.0){
			width += myRect.size.width;
			height += myRect.size.height;
			num++;
			rot += myRect.angle;
			centerX += myRect.center.x;
			centerY += myRect.center.y;
			drawContours(test.background,contour,i,Scalar(0.0,255.0,0.0),0.5);
		}
		//	drawContours(test.background,contour,i,Scalar(0.0,255.0,0.0),0.5);
		}
	}

	if(num != 0){

	width /= num;
	height /=num;
	rot /=num;

	centerX /=num;
	centerY /=num;
	}

	centerX = (centerX - 320.0) * TABLE_WIDTH / 640.0;
	centerY = (240.0 - centerY) * TABLE_HEIGHT / 480.0;

	curX += centerX;
	curY += centerY;

	incidentAngle = (atan(height/width));
	incidentAngle = incidentAngle / (2*M_PI) * -360.0;

	distanceFromTable = -1.0 * DEFAULT_DISTANCE / 2.0 * width / 640.0 - (DEFAULT_DISTANCE);

	//distanceFromTable = width / 2.0 / 640.0 * TABLE_WIDTH;

	//distanceFromTable *= -1.0 * DEFAULT_DISTANCE;

 	tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, curX, curY);

	tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, curX, curY, X_DIST, Y_DIST);

	fixedPts = tCam.extractCircles(cp);

	int k = waitKey();
	while(k != 121){
		if(k == 102){
	Point2f diff = tCam.findCircle(cp);

	x1y1 = fixedPts.at<double>(0);
	x2y1 = fixedPts.at<double>(1);
	x3y1 = fixedPts.at<double>(2);
	x1y2 = fixedPts.at<double>(3);
	x2y2 = fixedPts.at<double>(4);
	x3y2 = fixedPts.at<double>(5);
	x1y3 = fixedPts.at<double>(6);
	x2y3 = fixedPts.at<double>(7);
	x3y3 = fixedPts.at<double>(8);

	ans1 = x1y1 * diff.x + x2y1 * diff.y + x3y1 * 1.0;
	ans2 = x1y2 * diff.x + x2y2 * diff.y + x3y2 * 1.0;
	ans3 = x1y3 * diff.x + x2y3 * diff.y + x3y3 * 1.0;

	ans1 = ans1 / ans3;
	ans2 = ans2 / ans3;

	diffX = (320.0 - ans1) * TABLE_WIDTH / 640.0;
	diffY = (ans2 - 240.0) * TABLE_WIDTH / 640.0;
 	tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, 0.0, 0.0);

 	tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, diffX, diffY);

//	tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, diffX + curX, diffY + curY);
	//centerX = (diff.x - 350.0) * 23.5 / 640.0;
	//centerY = (diff.y - 240.0) * -17.55 / 480.0;
		
	}
		//w == up
		else if( k == 119){
			centerY += 0.12;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
		//a == left
				else if( k == 97){
			centerX -= 0.12;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
				//s == down
						else if( k == 115){
			centerY -= 0.12;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
						//d == right
								else if( k == 100){
			centerX += 0.12;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
														// up == angleincrease
								else if( k == 2490368){
			incidentAngle -= 0.35;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
														// left == rot increase
								else if( k == 2424832){
			rot += 0.2;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
														// down == angle decrease
								else if( k == 2621440){
			incidentAngle += 0.35;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
														// right == rot decrease
								else if( k == 2555904){
			rot -= 0.2;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
																						//== zoom in
								else if( k == 122){
			distanceFromTable += 0.5;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
														// == right
								else if( k == 120){
			distanceFromTable -= 0.5;
			tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY);
		}
	//centerX = (diff.x - 350.0) * 23.75 / 640.0;
	//centerY = (diff.y - 240.0) * -17.55 / 480.0;

	//tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerY, centerX);
	//centerX = (diff.x - 350.0) * 24.0 / 640.0;
	//centerY = (diff.y - 240.0) * -17.55 / 480.0;

	//tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, centerY, centerX);
	//}

	k = waitKey();
	}

	imshow("circle",test.background);

	waitKey();

	return Mat();

}