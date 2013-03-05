#include <opencv\cv.h>
#include <opencv\highgui.h>

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