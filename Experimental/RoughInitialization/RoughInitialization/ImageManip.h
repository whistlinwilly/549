#include <opencv\cv.h>
#include <opencv\highgui.h>

void findCircles(cv::Point2f* points, cv::Mat image, int numCircles, int minContourLength, int minCircleSize, float minRadialDifference);
cv::Mat extractDoubleCircleData(circleData* cdat, cv::Mat image, int minContourLength, int minCircleSize, float minRadialDifference, cv::Mat toReturn);
void orderCorners(cv::Point2f* corners);