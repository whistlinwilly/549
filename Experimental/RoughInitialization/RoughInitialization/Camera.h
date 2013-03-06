#include "DefinedObjects.h"
#include "ImageManip.h"


class Camera
{

public:
	Camera();
	cv::Mat init(void);
	cameraPerspective tryRotation(void);
	cameraPerspective findCorners(void);
	cv::Point2f findPoint(cv::Mat bg, cameraPerspective cp);
	cameraPerspective getBackground(cameraPerspective cp);
	cv::RotatedRect extractPoint(cameraPerspective cp);
	cv::Point3f findCircle(cameraPerspective cp);
	void extractPattern2(cameraPerspective cp);
	cv::Mat extractCircles(cameraPerspective cp);
	cv::VideoCapture fastCam;
	int initFastCam();
	cv::Mat grabFrame();
	cv::Mat grabFrameWithPerspective(cameraPerspective cp);
};