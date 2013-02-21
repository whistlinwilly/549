#include <opencv\cv.h>
#include <opencv\highgui.h>

class ControlBoard
{
public:
	ControlBoard();
	void runObjectTracking();
	cv::Mat classifyObject(std::vector<cv::Point> contour);
	void init(void);
};