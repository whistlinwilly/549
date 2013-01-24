#include <SDL.h>
#include <SDL_opengl.h>
#include <opencv\cv.h>

#define WIDTH 1280
#define HEIGHT 800
#define BPP 4
#define DEPTH 32

class Projector
{

public:
	Projector();
	void init(void);
	void renderFrame(cv::Point2f point);
	void renderDisk(cv::Point2f point, cv::Mat transform);
	void renderStar(cv::Point2f point0, cv::Point2f point1, cv::Point2f point2, cv::Point2f point3, cv::Point2f point4, cv::Point2f point5, cv::Point2f point6, cv::Point2f point7, cv::Point2f point8);
	void destroy(void);
};