#include "ControlBoard.h"
#include "Projector.h"
#include "Camera.h"
#include "Server.h"
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>
#include <algorithm>


#define _USE_MATH_DEFINES

#include <math.h>


using namespace std;
using namespace cv;

Server* myServer;

void projInit( int event, int x, int y, int flags, void* param );
void camRotationTest(void);
Mat getPointMapping(void);
Mat getPerspectiveMapping(void);
float myDistanceFromTable;
float myIncidence;
float myCenterX;
float myCenterY;
float myTwist;
float myRot;

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

		



	Mat myMat;

	//FROM MY TABLE INIT
	RotatedRect myRect;
	cameraPerspective test;
	cameraPerspective nTest;
	curWindow= "projInit";
	namedWindow(curWindow, CV_WINDOW_AUTOSIZE);
	setMouseCallback(curWindow, projInit, (void*) NULL);
	imshow(curWindow, imread("projInit.jpg"));
	waitKey();

	//MARTINS TABLE INIT
	//tProj.init();
	//tProj.renderFrame(Point2f(0.0,0.0));
	//tCam.initFastCam();
	//runMartinsInit();

	myServer = new Server();

	Mat newFrame = tCam.grabFrameWithPerspective(cp);
	Mat trans, foreground;
	tableBGsub = new BackgroundSubtractorMOG2(0,100,true);
	tableBGsub->operator()(newFrame,foreground,0.001);
	myMat = getPerspectiveMapping();
	tProj.renderFrame(Point2f(0,0));
	//tCam.initFastCam();
	namedWindow("ROI",WINDOW_AUTOSIZE);
	runObjectTracking();




	//myMat = getPointMapping();

}

bool byX(Point2f pt1, Point2f pt2) {
  if (pt1.x <= pt2.x) return true;
  else return false;
}

bool byY(Point2f pt1, Point2f pt2) {
  if (pt1.y <= pt2.y) return true;
  else return false;
}

void ControlBoard::runMartinsInit(void){

	Mat image, merged, gray, canny, copy;
	vector<Mat> bgr_planes;
	vector<Vec4i> lines;
	vector<Vec4d> extendLines;
	vector<float> angleLines;
	vector<Point2f> intersections;
	int numOfHoughLines = 1000;

	image = tCam.grabFrame();
	copy = image.clone();

	split( image, bgr_planes );

	equalizeHist(bgr_planes[0], bgr_planes[0]);
	equalizeHist(bgr_planes[1], bgr_planes[1]);
	equalizeHist(bgr_planes[2], bgr_planes[2]);

	merge(bgr_planes, merged);

	cvtColor(merged, gray, CV_RGB2GRAY);

	imshow("First Checkpoint", gray);
	waitKey();


	while (numOfHoughLines >= 20) {
    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat hist;
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    int total = image.rows * image.cols;
    int halfTotal = total / 2;
    float current = 0;
    for (int i = 0; i < histSize; i ++) {
      current += hist.at<float>(i);
      if (current >= halfTotal) {
        current = i;
        break;
      }
    }

    Canny(gray, canny, 0.66*current, 1.33*current, 3, true);
    
	imshow("SecondCheckpoint", canny);
	waitKey();

    int houghLinesPThreshold = 100;
    int minLineLength = 10;
    int maxLineGap = 10;

    HoughLinesP(canny, lines, 3, CV_PI/180, houghLinesPThreshold, minLineLength, maxLineGap);
    numOfHoughLines = lines.size();
    GaussianBlur(gray,gray,Size(3,3),0,0,BORDER_DEFAULT);
	}

	for(int i = 0; i < lines.size(); i++){
		Vec4i l = lines[i];
		line(copy, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
	}

	imshow("ThirdCheckpoint", copy);
	waitKey();

	for (int i = 0; i < lines.size(); i++){
    Vec4i l = lines[i];
    float x1 = 1.0*l[0], y1 = 1.0*l[1], x2 = 1.0*l[2], y2 = 1.0*l[3];
    Vec4f temp;
    float angle = (180.0/M_PI)*atan2((y2-y1),(x2-x1));
  //  cout << "at i = " << i << " angle is " << angle << " and " << tan(angle/180.0*CV_PI) << endl;
    float a = cos(angle/180.0*CV_PI), b = sin(angle/180.0*CV_PI);
    temp[0] = x1 - 10000*a;
    temp[1] = y1 - 10000*b;
    temp[2] = x1 + 10000*a;
    temp[3] = y1 + 10000*b;
	extendLines.push_back(temp);
    angleLines.push_back(angle);
	}

	for(size_t i = 0; i < extendLines.size(); i++ ){
    for(size_t j = 0; j < extendLines.size(); j++){
      Vec4f line1 = extendLines[i];
      Vec4f line2 = extendLines[j];
      if(abs(abs(angleLines[i]) - abs(angleLines[j])) > 30){
  //      cout << "angle i is " << angleLines[i] << endl;
  //      cout << "angle j is " << angleLines[j] << endl;
        Point2f intersection = computeIntersection(line1, line2);
        intersections.push_back(intersection);
      }
    }
  }

	  vector<Point2f> topLeft, topRight, bottomLeft, bottomRight;
  for(vector<Point2f>::iterator i = intersections.begin(); i != intersections.end(); ++i){
    Point2f pt = *i;
    float x = pt.x, y = pt.y;
	if (x < 0.25 * image.cols && y < 0.25 * image.rows){
      topLeft.push_back(pt);
	} else if (x < 0.25 * image.cols && y > 0.75 * image.rows) {
      bottomLeft.push_back(pt);
	} else if (x > 0.75 * image.cols && y < 0.25 * image.rows) {
      topRight.push_back(pt);
	} else if (x > 0.75 * image.cols && y > 0.75 * image.rows) {
      bottomRight.push_back(pt);
    } else {
      continue;
    }
  }

  Point2f tl = approxCorner(topLeft);
  Point2f tr = approxCorner(topRight);
  Point2f bl = approxCorner(bottomLeft);
  Point2f br = approxCorner(bottomRight);

  circle(copy, tl, 1, Scalar(255, 0, 0), 3);
  circle(copy, tr, 1, Scalar(255, 0, 0), 3);
  circle(copy, bl, 1, Scalar(255, 0, 0), 3);
  circle(copy, br, 1, Scalar(255, 0, 0), 3);

  imshow("FINAL!!!", copy);
  waitKey();

}

cv::Point2f ControlBoard::approxCorner(vector<Point2f> pts) {
  float medianX = getMedianX(pts);
  float medianY = getMedianY(pts);

  float error = 10.0, sumX = 0.0, sumY = 0.0;
  int count = 0;

  for (int i = 0; i < pts.size(); i ++) {
    Point2f pt = pts[i];
    float dx = abs(pt.x - medianX);
    float dy = abs(pt.y - medianY); 
    if (sqrt(pow(dx,2) + pow(dy, 2)) < error) {
      sumX += pt.x;
      sumY += pt.y;
      count ++;
    }
  }

  return Point2f(sumX/count, sumY/count);
}

float ControlBoard::getMedianX (vector<Point2f> pts) {
	std::vector<float> x;

	for(int i = 0; i < pts.size(); i++){
		x.push_back(pts[i].x);
	}

	sort(x.begin(), x.end());
  float medianX = x[pts.size()/2];
  return medianX;
}

float ControlBoard::getMedianY (vector<Point2f> pts) {
	std::vector<float> y;

	for(int i = 0; i < pts.size(); i++){
		y.push_back(pts[i].y);
	}

	sort(y.begin(), y.end());
  float medianY = y[pts.size()/2];
  return medianY;
}

cv::Point2f ControlBoard::computeIntersection(Vec4f line1, Vec4f line2){
    vector<Point2f> p1 = lineToPointPair(line1);
    vector<Point2f> p2 = lineToPointPair(line2);

    float denom = (p1[0].x - p1[1].x)*(p2[0].y - p2[1].y) - (p1[0].y - p1[1].y)*(p2[0].x - p2[1].x);
    Point2f intersect(((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].x - p2[1].x) -
                       (p1[0].x - p1[1].x)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom,
                      ((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].y - p2[1].y) -
                       (p1[0].y - p1[1].y)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom);

    return intersect;
}

vector<Point2f> ControlBoard::lineToPointPair(Vec4f line){
    vector<Point2f> points;

    points.push_back(Point2f(line[0], line[1]));
    points.push_back(Point2f(line[2], line[3]));

    return points;
}

void ControlBoard::runObjectTracking(void){
	Mat actual;
	Mat foreground;
	Mat frame;
	Mat canny;
	Mat thresh2;
	Mat thresh;
	Mat contours;
	Mat canny2;
	Mat trans;
	vector<Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > contour;

	
	
	
	frame = tCam.grabFrameWithPerspective(cp);
	threshold(frame, trans, 60.0, 255.0,THRESH_TRUNC);
	BackgroundSubtractorMOG2* bg = new BackgroundSubtractorMOG2(0,100,true);
	bg->operator()(trans,foreground,0.001);

	while(1){
		frame = tCam.grabFrameWithPerspective(cp);
		threshold(frame, trans, 60.0, 255.0,THRESH_TRUNC);
		bg->operator()(trans,foreground,0.001);
		threshold(foreground, thresh, 245.0, 255.0,THRESH_BINARY);
		Canny(thresh, canny, 100, 250, 3);

		findContours( canny, contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );

			for(int i=0; i<contour.size(); i++){
				if(contour[i].size() > 500){
					Mat myTest = classifyObject(contour[i]);
					imshow("WINDOW",myTest);
				}
			}
					imshow("FORE", trans);
					imshow("WINDOW2",canny);
			waitKey(10);
	}

}

Mat ControlBoard::classifyObject(std::vector<cv::Point> contour){

	Mat frame;
	Mat grayFrame;
	std::vector<int> hull;
	std::vector<Vec4i> defects; 
	std::vector<cv::Point> points;
	float x1y1, x2y1, x3y1,x1y2,x2y2,x3y2,x1y3,x2y3,x3y3;
	float ans1, ans2, ans3;

		RotatedRect myRect;
		myRect = fitEllipse(contour);
		convexHull(contour,hull,false);
		convexityDefects(contour,hull,defects);

		frame = tCam.grabFrameWithPerspective(cp);
		IplImage* myImage = new IplImage(frame);

		
		for(int i = 0; i < defects.size(); i++){
			Vec4i curDef = defects.at(i);
			int start = curDef[0];
			int finish = curDef[1];
			//cvLine(myImage,contour[start],contour[finish],Scalar(255.0,255.0,255.0),1.0);
			points.push_back(contour[start]);
			points.push_back(contour[finish]);

		}

		RotatedRect newRect = minAreaRect(points);


		Rect roi = boundingRect(points);

		Point3f diff;
		diff.x= newRect.center.x;
		diff.y = newRect.center.y;
		diff.z = newRect.angle;

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

	if(newRect.size.height > newRect.size.width)
		diff.z += 90;

	diff.z = diff.z / -360 * 2 * 3.14159;

	//Mat roiFrame = frame(roi);

	//cvtColor(roiFrame, grayFrame, CV_RGB2GRAY);
	//
	//imshow("ROI", grayFrame);
	//int key = waitKey();

	//if(key == 121){

	//	if(!hasTrainingImage){
	//		train = contour;
	//		hasTrainingImage = true;
	//	}
	//	else{
	//		
	//		double matched = matchShapes(train, contour, CV_CONTOURS_MATCH_I2, 0);
	//		matched +=10;

	//	}

	tProj.renderBathtub(myDistanceFromTable, myIncidence, myRot, myTwist, myCenterX, myCenterY, ans1, ans2, diff.z);

//	}

		       Point2f rect_points[4]; newRect.points( rect_points );
       for( int j = 0; j < 4; j++ )
          line( frame, rect_points[j], rect_points[(j+1)%4], Scalar(255.0,0.0,0.0), 2, 8 );

		

			/*imshow("WINDOW",Mat(myImage));
			waitKey();*/

		//return Mat(myImage).clone();
	   return frame.clone();
}

void projInit( int event, int x, int y, int flags, void* param ){
	switch( event ){
		case CV_EVENT_LBUTTONUP:
			//tProj.init();
			//tProj.renderFrame(Point2f(0.0,0.0));
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

	//OBJECT TRACKING VARIABLES
	Mat background;
	Mat foreground;
	//END VARS

	cameraPerspective test;
	Mat bThresh, bCan, grey;
	vector<Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > contour;
	RotatedRect myRect;


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
	float twist = 0.0;
	
float distanceFromTable = DEFAULT_DISTANCE;

	//tProj.renderInitPattern();

Mat bgFrame, bgThresh, bgForeground;

	bgFrame = tCam.grabFrameWithPerspective(cp);
	tableBGsub->operator()(bgFrame,bgForeground,0.001);

	myServer->sendString("0");

	myServer->confirm();

	waitKey(100);

	bgFrame = tCam.grabFrameWithPerspective(cp);

	imshow("FRAME GRABBED", bgFrame);
	waitKey();

	tableBGsub->operator()(bgFrame,bgForeground,0.001);

	//Mat back;

	//tableBGsub->getBackgroundImage(back);

	imshow("BACKGROUND MODEL", bgForeground);
	waitKey();

	threshold(bgForeground, bThresh, 240.0, 255.0,THRESH_BINARY);

	imshow("BACKGROUND THRESHOLD", bThresh);
	waitKey();

	Canny(bThresh, bCan, 100, 250, 3);

	imshow("BACKGROUND CANNY", bCan);
	waitKey();

	//OLD CODE WITH THRESHOLDING
	//test = tCam.getBackground(cp);

	////Works for brighter projector with multicolored test pattern
	//IplImage rgb = test.background;

	//imshow("TEST", test.background);
	//waitKey();


	//IplImage* r = cvCreateImage( cvGetSize(&rgb), rgb.depth,1 );
	//IplImage* g = cvCreateImage( cvGetSize(&rgb), rgb.depth,1 );
	//IplImage* b = cvCreateImage( cvGetSize(&rgb), rgb.depth,1 );

	//cvSplit(&rgb,b,g,r,NULL);

	//Mat red = r;
	//Mat blue = b;
	//Mat green = g;

	//cvtColor(test.background, grey, CV_RGB2GRAY);

	/*threshold(blue, bThresh, 240.0, 255.0,THRESH_BINARY);

	imshow("circle", bThresh);

	waitKey();

	Canny(bThresh, bCan, 100, 250, 3);

	imshow("circle", bCan);

	waitKey();*/



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
			//drawContours(test.background,contour,i,Scalar(0.0,255.0,0.0),1.0);
			ellipse(bgFrame, myRect.center, cv::Size(myRect.size.width/2, myRect.size.height/2), myRect.angle, 0.0, 360.0, Scalar(0.0,0.0,255.0),2.0);
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

	if(rot >= 90){
		rot -= 180;
	}

	centerX = (centerX - 320.0) * TABLE_WIDTH / 1280.0;
	centerY = (centerY - 240.0) * TABLE_HEIGHT / 960.0;

	curX += centerX;
	curY += centerY;

	incidentAngle = (acos(width/height));
	incidentAngle = incidentAngle / (2*M_PI) * -360.0;

	//distanceFromTable = -1.0 * DEFAULT_DISTANCE / 2.0 * width / 640.0 - (DEFAULT_DISTANCE);

	//FIXING FACTOR
	//width -=5.0;


	distanceFromTable = (width / 2 / -TABLE_X * TABLE_WIDTH) * DEFAULT_DISTANCE;

	//distanceFromTable = width / 2.0 / 640.0 * TABLE_WIDTH;

	//distanceFromTable *= -1.0 * DEFAULT_DISTANCE;

	imshow("CONTOUR", bgFrame);
	waitKey();

	float oldIncidentAngle = incidentAngle;
	incidentAngle=0.0;

		float oldrot = rot;
	rot=0.0;
		centerX = 0;
		centerY = 0;
	int k = waitKey();
	while(k != 121){

		
		if(k == 102){
	Point3f diff = tCam.findCircle(cp);

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

	diffX = (ans1 - 320.0) * TABLE_WIDTH / 1280.0;
	diffY = (ans2 - 240.0) * TABLE_WIDTH / 960.0;
	tProj.renderPatternWithPerspective(-50.0, incidentAngle, rot, curX, curY, 0.0);

	tProj.renderPatternWithPerspective(-50.0, incidentAngle, rot, curX + diffX, curY + diffY, 0.0);

//	tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, diffX + curX, diffY + curY);
	//centerX = (diff.x - 350.0) * 23.5 / 640.0;
	//centerY = (diff.y - 240.0) * -17.55 / 480.0;
		
	}
		//w == up
		else if( k == 119){
			centerY += 0.12;
		tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
		//a == left
				else if( k == 97){
			centerX -= 0.12;
		tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
				//s == down
						else if( k == 115){
			centerY -= 0.12;
		tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
						//d == right
								else if( k == 100){
			centerX += 0.12;
	tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
														// up == angleincrease
								else if( k == 2490368){
			incidentAngle -= 0.45;
	tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
														// left == rot increase
								else if( k == 2424832){
			rot += 0.25;
	tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
														// down == angle decrease
								else if( k == 2621440){
			incidentAngle += 0.45;
		tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
														// right == rot decrease
								else if( k == 2555904){
			rot -= 0.25;
		tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
																						//== zoom in
								else if( k == 122){
			distanceFromTable += 0.5;
tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
														// == right
								else if( k == 120){
			distanceFromTable -= 0.5;
		tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}

										else if( k == 44){
			twist -= 0.25;
			tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
										else if( k == 46){
			twist += 0.25;
			tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}

										else if( k == 105){
			incidentAngle = oldIncidentAngle;
			tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
		}
										else if( k == 114){
			twist = oldrot;
			tProj.renderInitWithPerspective(distanceFromTable, incidentAngle, rot, centerX, centerY, twist);
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

 		tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, 0, 0, 8, 6, twist);


	k = waitKey();
	while(k != 121){

		
		if(k == 102){
	Point3f diff = tCam.findCircle(cp);

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

	diffX = (ans1 - 320.0) * TABLE_WIDTH / 1280.0;
	diffY = (ans2 - 240.0) * TABLE_WIDTH / 960.0;
	tProj.renderPatternWithPerspective(-50.0, incidentAngle, rot, curX, curY, 0.0);

	tProj.renderPatternWithPerspective(-50.0, incidentAngle, rot, curX + diffX, curY + diffY, 0.0);

//	tProj.renderPatternWithPerspective(distanceFromTable, incidentAngle, rot, diffX + curX, diffY + curY);
	//centerX = (diff.x - 350.0) * 23.5 / 640.0;
	//centerY = (diff.y - 240.0) * -17.55 / 480.0;
		
	}
		//w == up
		else if( k == 119){
			centerY += 0.12;
		tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
		//a == left
				else if( k == 97){
			centerX -= 0.12;
		tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
				//s == down
						else if( k == 115){
			centerY -= 0.12;
		tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
						//d == right
								else if( k == 100){
			centerX += 0.12;
	tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
														// up == angleincrease
								else if( k == 2490368){
			incidentAngle -= 0.45;
	tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
														// left == rot increase
								else if( k == 2424832){
			rot += 0.25;
	tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
														// down == angle decrease
								else if( k == 2621440){
			incidentAngle += 0.45;
		tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
														// right == rot decrease
								else if( k == 2555904){
			rot -= 0.25;
		tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
																						//== zoom in
								else if( k == 122){
			distanceFromTable += 0.5;
tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
														// == right
								else if( k == 120){
			distanceFromTable -= 0.5;
		tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}

										else if( k == 44){
			twist -= 0.25;
			tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
										else if( k == 46){
			twist += 0.25;
			tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}

										else if( k == 105){
			incidentAngle = oldIncidentAngle;
			tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
		}
										else if( k == 114){
			twist = oldrot;
			tProj.renderInitPattern2(distanceFromTable, incidentAngle, rot, centerX, centerY, 8, 6, twist);
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


	cp.x = centerX;
	cp.y = centerY;


	fixedPts = tCam.extractCircles(cp);

	myDistanceFromTable = distanceFromTable;
	myIncidence = incidentAngle;
	myRot = rot;
	myCenterY = centerY;
	myCenterX = centerX;
	myTwist = twist;

	//BackgroundSubtractor* bg = new BackgroundSubtractorMOG2(1,64,true);
	//Mat actual;

	//while(1){

	//	//background = tCam.grabFrame();
	//	//bg->operator()(background,foreground,0.05);
	//	//bg->getBackgroundImage(actual);
	//	//imshow("RUNNING FOREGROUND",foreground);
	//	//imshow("RUNNING BACKGROUND",actual);
	//	//waitKey(10);

	//	//OLD WORKING OBJECT TRACKING
 //	Point3f diff = tCam.findCircle(cp);

	////tProj.renderBathtub(distanceFromTable, incidentAngle, rot, twist, centerX, centerY,0,0);

	//x1y1 = fixedPts.at<double>(0);
	//x2y1 = fixedPts.at<double>(1);
	//x3y1 = fixedPts.at<double>(2);
	//x1y2 = fixedPts.at<double>(3);
	//x2y2 = fixedPts.at<double>(4);
	//x3y2 = fixedPts.at<double>(5);
	//x1y3 = fixedPts.at<double>(6);
	//x2y3 = fixedPts.at<double>(7);
	//x3y3 = fixedPts.at<double>(8);

	//ans1 = x1y1 * diff.x + x2y1 * diff.y + x3y1 * 1.0;
	//ans2 = x1y2 * diff.x + x2y2 * diff.y + x3y2 * 1.0;
	//ans3 = x1y3 * diff.x + x2y3 * diff.y + x3y3 * 1.0;

	//ans1 = ans1 / ans3;
	//ans2 = ans2 / ans3;

	//tProj.renderBathtub(distanceFromTable, incidentAngle, rot, twist, centerX, centerY, ans1, ans2, diff.z + 90);

	//}
	

	return Mat();

}