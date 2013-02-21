#include "DefinedObjects.h"
#include "ImageManip.h"

#define X_DIST (7)
#define Y_DIST (5)

#ifndef TABLE_WIDTH
#define TABLE_WIDTH (23.5)
#endif 

#ifndef TABLE_HEIGHT
#define TABLE_HEIGHT (17.5)
#endif 


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
};