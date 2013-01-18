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
	void destroy(void);
};