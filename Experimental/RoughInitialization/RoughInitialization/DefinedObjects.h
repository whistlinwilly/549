#include <opencv\cv.h>

typedef struct{
	cv::Point2f points[8];
	cv::Point2f center;
	cv::Mat background;
	cv::Mat pic;
	float angle;
	cv::Mat perspectiveWarp;

}cameraPerspective;