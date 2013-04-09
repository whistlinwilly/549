#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "ServerNetwork.h"
#include "Camera.h"

class Initialization
{
public:
	Mapper mapper;
	ServerNetwork* sn;
	Camera tCam;
	BackgroundSubtractorMOG2* tableBGsub;
	static unsigned int projectorsConnected;
	static char* recvbuf;
	char* curWindow;
	CameraPerspective cp;
	circleData* cDatSmall;
	circleData* cDatBig;
	Mat bgFrame;
	Mat bgForeground;
	Mat bgThresh;

	Initialization();
	void init(int camNumber);
	void connectToProjectors(char* ip, int port);
	void camRotation(void);
	void bgSubtraction(void);
	void readInit1(void);
	void readInit2(void);
	void computeAndSendPerspectives(void);
};

