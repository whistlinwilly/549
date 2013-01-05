#include <stdio.h>
#include <iostream>

#include "Projector.h"
#include "Camera.h"

Projector tPro;
Camera tCam;


int main(int argc, char* argv[])
{
	tPro.init();
	tPro.renderFrame();
	Sleep(10);
	tCam.init();
	Sleep(5000);
	tPro.destroy();
	return 0;
}



