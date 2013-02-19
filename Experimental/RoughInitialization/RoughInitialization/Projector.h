#include <SDL.h>
#include <SDL_opengl.h>
#include <opencv\cv.h>

#ifndef DEFAULT_DISTANCE
#define DEFAULT_DISTANCE (36.0)
#endif 

#ifndef TABLE_WIDTH
#define TABLE_WIDTH (23.5)
#endif 

#ifndef TABLE_HEIGHT
#define TABLE_HEIGHT (17.5)
#endif 

#ifndef PROJ_FOV
#define PROJ_FOV (20.5)
#endif 

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
	void renderDisk();
	void renderStar(cv::Point2f point0, cv::Point2f point1, cv::Point2f point2, cv::Point2f point3, cv::Point2f point4, cv::Point2f point5, cv::Point2f point6, cv::Point2f point7, cv::Point2f point8);
	void destroy(void);
	void renderInitPattern();
	void renderPatternWithPerspective(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY, float twist);
	void renderInitWithPerspective(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY, float twist);
	void renderInitPattern2(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY, float xDist, float yDist, float twist);
	GLuint codyTex;			// This is a handle to our texture object
	GLuint wallTex;
};