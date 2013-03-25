#include <opencv\cv.h>
#include <opencv\highgui.h>

#ifndef CIRCLE_DATA
#define CIRCLE_DATA
typedef struct{
	float c1x;
	float c1y;
	float c2x;
	float c2y;
	float c1w;
	float c1h;
	float c2w;
	float c2h;
	float c1r;
	float c2r;
}circleData;
#endif

void findCircles(cv::Point2f* points, cv::Mat image, int numCircles, int minContourLength, int minCircleSize, float minRadialDifference);
cv::Mat extractDoubleCircleData(circleData* cdat, cv::Mat image, int minContourLength, int minCircleSize, float minRadialDifference, cv::Mat toReturn);
void orderCorners(cv::Point2f* corners);