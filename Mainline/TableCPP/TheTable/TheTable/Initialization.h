#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "ServerNetwork.h"
#include "Camera.h"
#include "Mapper.h"
#include <opencv2\opencv.hpp>

using namespace cv;

class Initialization
{
public:
	Mapper *mapper;
	Mapper *mapper2;
	ServerNetwork* sn;
	Camera tCam;
	BackgroundSubtractorMOG2* tableBGsub;
	unsigned int projectorsConnected;
	char* recvbuf;
	char* curWindow;
	CameraPerspective cp;
	Mat bgFrame;
	Mat bgForeground;
	Mat foreground;
	Mat frame;
	Mat thresh;

	Initialization();
	void init(int camNumber);
	void connectToProjectors(char* ip, int port);
	void camRotation(void);
	void bgSubtraction(void);
	void readInit1(int projNum);
	void readInit2(int projNum);
	void computeAndSendPerspectives(int projNum);
};

