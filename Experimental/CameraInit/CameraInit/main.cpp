#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>

using namespace std;
using namespace cv;


int main(){

	//idea for initialization...pulsing water? should fill the screen and have circular shape!

	//Eventually this should be incorporated into a camera class
	//this is the first attempt at an initialization function which simply returns
	//the perspective transformation matrix. ideally we can use this to center and mirror the image
	//so as to prepare for the projector

	//First a gaussian adaptive threshold is applied
	//Erode might work alright too? try both


	//adaptiveThreshold(m, n, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, variableConstant?);

	//code below was used to take original images

	/*
	VideoCapture cap;
	cap.open(0);
	cout<<cap.isOpened()<<endl;

	Mat m;

	int rh = 255, rl = 100, gh = 255, gl = 0, bh = 70, bl = 0;

    string windowName = "Camera Init";
    namedWindow(windowName);

    createTrackbar("rh", windowName, &rh, 255);
    createTrackbar("rl", windowName, &rl, 255);
    createTrackbar("gh", windowName, &gh, 255);
    createTrackbar("gl", windowName, &gl, 255);
    createTrackbar("bh", windowName, &bh, 255);
    createTrackbar("bl", windowName, &bl, 255);

    // for dilation
    Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));

    Mat bgIsolation;
    int key = 0;
	int i = 0;
	char filename[25];
    do
    {

		cap>>m;

        inRange(m, Scalar(bl, gl, rl), Scalar(bh, gh, rh), bgIsolation);

        bitwise_not(bgIsolation, bgIsolation);

        erode(bgIsolation, bgIsolation, Mat());
     //   dilate(bgIsolation, bgIsolation, element);

        imshow(windowName, bgIsolation);
        key = waitKey(33);
		if(key == 32){
			sprintf(filename,"img%d.jpg",i++);
			cout << "Writing image: " << filename;
			imwrite(filename,bgIsolation);
		}
    } while((char)key != 27);

    waitKey();

    return 0;
	*/

	Mat m, n,a ;


	string windowName = "Camera Init";
    namedWindow(windowName);

	m = imread("img7.jpg", -1);
	
	Canny(m, n, 50, 200, 3);


	//We apply a hough transform to extract lines
	//this is the lazy way, we should really use a standard hough transform and then
	//de-hough it. fine for now

	 vector<Vec4i> lines;
  HoughLinesP(n, lines, 1, CV_PI/180, 50, 80, 10 );
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( n, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
  }

  //convert lines to vectors
  // consolidate? picture seems to be aliasing

  //check for intersections of perpendicular vectors (find 4!)

  //use getPerspectiveTransform()....done!

	imshow(windowName, n);


	waitKey();


}