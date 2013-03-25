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

class ControlBoard
{
public:
	ControlBoard();
	void runObjectTracking();
	cv::Mat classifyObject(std::vector<cv::Point> contour);
	cv::Point2f computeIntersection(cv::Vec4f line1, cv::Vec4f line2);
	void runMartinsInit();
	cv::Point2f approxCorner(std::vector<cv::Point2f> points);
	float getMedianX(std::vector<cv::Point2f> points);
	float getMedianY(std::vector<cv::Point2f> points);
	std::vector<cv::Point2f> lineToPointPair(cv::Vec4f line);
	void init(void);
};