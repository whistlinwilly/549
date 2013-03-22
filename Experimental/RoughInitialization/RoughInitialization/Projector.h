#include <SDL.h>
#include <SDL_opengl.h>
#include <opencv\cv.h>

//FOV of projector
#ifndef PROJ_FOV
#define PROJ_FOV (18.2)
#endif 

//SDL constants for opengl image plane on second display
#define WIDTH 1280
#define HEIGHT 800
#define BPP 4
#define DEPTH 32

//Distance for second test patter (from center)
#define X_DIST (7)
#define Y_DIST (5)

//Distance from image plane for first orthographic projection
#ifndef DEFAULT_DISTANCE
#define DEFAULT_DISTANCE (24.0)
#endif 

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
	void renderBathtub(float distanceFromTable, float incidentAngle,float  rot,float  twist,float  xCenter,float  yCenter, float distX, float distY, float shapeAngle); 
	GLuint createTexture(char* filename);
	GLuint* texture;
};