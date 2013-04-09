#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>
#include "Tracker.h"

using namespace std;
using namespace cv;

Tracker::Tracker(){
}

void Tracker::runObjectTracking(void){
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

vector<Point2f> Tracker::lineToPointPair(Vec4f line){
    vector<Point2f> points;

    points.push_back(Point2f(line[0], line[1]));
    points.push_back(Point2f(line[2], line[3]));

    return points;
}

Mat Tracker::classifyObject(std::vector<cv::Point> contour){

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

	tProj.renderBathtub(myDistanceFromTable, myIncidence, myRot, myTwist, myCenterX, myCenterY, ans1, ans2, diff.z);

		       Point2f rect_points[4]; newRect.points( rect_points );
       for( int j = 0; j < 4; j++ )
          line( frame, rect_points[j], rect_points[(j+1)%4], Scalar(255.0,0.0,0.0), 2, 8 );

	   return frame.clone();
	}
