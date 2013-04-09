#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>

class Tracker{
private:
	//cv::Mat classifyObject(std::vector<cv::Point> contour);
	//std::vector<cv::Point2f> lineToPointPair(cv::Vec4f line);
public:
	cv::Mat classifyObject(std::vector<cv::Point> contour);
	std::vector<cv::Point2f> lineToPointPair(cv::Vec4f line);
	Tracker();
	void runObjectTracking(void);
};