#include <opencv\cv.h>
#include <opencv\highgui.h>

void findCircles(cv::Point2f* points, cv::Mat image, int numCircles, int minContourLength, int minCircleSize, float minRadialDifference);
void orderCorners(cv::Point2f* corners);