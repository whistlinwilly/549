#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>

using namespace std;
using namespace cv;


bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2,
                      Point2f* r)
{
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/1e-3)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    *r = o1 + d1 * t1;
    return true;
}

int main(){

	VideoCapture cap;
	Mat cam, grey, gat, ht, dht, out, gauss, can, newMat;
	int thresh = 1;
	Point2f corners[4];
	Point2f cornerSquare[4] = {Point2f(0,0),Point2f(0,480),Point2f(480,480),Point2f(480,0)};
	int found = 0;

	try{
	cap.open(0);
	}
	catch(Exception e){
		cout << "Couldn't open webcam\n" << endl;
		waitKey();
		exit(0);
	}

	cap>>cam;
	ht = cam;

	imshow("Test", cam);
	waitKey();

	cvtColor( cam, grey, CV_RGB2GRAY);

	imshow("Test", grey);
//	waitKey();

	GaussianBlur( grey, gauss, Size( 3, 3 ), 0, 0 );
	imshow("Test", gauss);
//	waitKey();

	Canny(gauss, can, 10, 250, 3);
	imshow("Test", can);
//	waitKey();

	vector<vector<Point> > contours;
	findContours(can, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	RotatedRect box = minAreaRect(contours[0]);

	Mat rot_mat = getRotationMatrix2D(box.center,box.angle,1);

	Mat rotated;

	warpAffine(cam, rotated, rot_mat, cam.size(), INTER_CUBIC);

  imshow("Test", rotated);
//	waitKey();

	cvtColor( rotated, grey, CV_RGB2GRAY);

	imshow("Test", grey);
//	waitKey();

	GaussianBlur( grey, gauss, Size( 3, 3 ), 0, 0 );
	imshow("Test", gauss);
//	waitKey();

	Canny(gauss, can, 10, 250, 3);
	imshow("Test", can);
//	waitKey();

	vector<Vec2f> lines;
	HoughLines(can, lines, 1, CV_PI/180, 25, 0, 0 );

	float rho;
	float theta;
	double a, b;
	double x0, y0;
	Point2f pt1, pt2, pt3, pt4;
	Point2f pt5[4];

	for(size_t i = 0; i < 4; i++){
		
		rho = lines[i][0];
		theta = lines[i][1];
		a = cos(theta);
		b = sin(theta);
		x0 = a * rho;
		y0 = b * rho;
		pt1 = Point(x0 + 1000*(-b), y0 + 1000*(a));
		pt2 = Point(x0 - 1000*(-b), y0 - 1000*(a));

		for(size_t j = i + 1; j < 4; j++){
			rho = lines[j][0];
			theta = lines[j][1];
			a = cos(theta);
			b = sin(theta);
			x0 = a * rho;
			y0 = b * rho;
			pt3 = Point(x0 + 1000*(-b), y0 + 1000*(a));
			pt4 = Point(x0 - 1000*(-b), y0 - 1000*(a));

			if(intersection(pt1, pt2, pt3, pt4, &corners[found]) == true){
				found++;
			}
			
		}
	}

	//the below is a super shitty method of positioning the corners in the array so
	//corners[0] is top left, corners[1] bottom left, corners[2] bottom right, corners[3] top right

	int min = 500;
	int max = -500;
	int minI = 0;
	int maxI = 0;

	for(int i = 0; i < 4; i++){
		if(corners[i].x + corners[i].y < min){
			min = corners[i].x + corners[i].y;
			minI = i;
		}
		if(corners[i].x + corners[i].y > max){
			max = corners[i].x + corners[i].y;
			maxI = i;
		}
	}

	Point2f temp = corners[0];
	corners[0] = corners[minI];
	corners[minI] = temp;

	temp = corners[2];
	corners[2] = corners[maxI];
	corners[maxI] = temp;

	if(corners[1].y < corners[1].x){
		temp = corners[1];
		corners[1] = corners[3];
		corners[3] = temp;
	}

	for(int i=0; i<4;i++){
	circle(rotated,corners[i],5,Scalar(255,0,0),3);
	imshow("Test", rotated);
//	waitKey();
	}


	Mat homey, nullMat;

	homey = getPerspectiveTransform(corners,cornerSquare);

	warpPerspective(rotated,out,homey,rotated.size());


/*	
	for( size_t i = 0; i < tempLines.size(); i++ )
    {
        float rho = tempLines[i][0];
        float theta = tempLines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( rotated, pt1, pt2, Scalar(0,0,255), 1, 8 );
		imshow("Test", rotated);
		waitKey();
    }
	
*/
/*
	  for( size_t i = 0; i < tempLines.size(); i++ )
  {
    Vec4i l = tempLines[i];
    line( rotated, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,i * 50,0), 1, CV_AA);
  }
  */


	imshow("Test", out);
	waitKey();




	//adaptiveThreshold(grey, gat, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, -10.0);
/*
	imshow("Test", can);
	waitKey();

	GaussianBlur( can, gauss, Size( 9, 9 ), 0, 0 );
	imshow("Test", gauss);
	waitKey();
	
	

	vector<Vec4i> tempLines;
  HoughLinesP(gauss, tempLines, 1, CV_PI/180, 380, 250, 50 );

  cout << "FOUND " << tempLines.size() << " LINES...CONSOLIDATING\n";
  
  for( size_t i = 0; i < tempLines.size(); i++ )
  {
	  if(linesFound == 4)
		  break;

	  newLine = false;
	  Vec4i l = tempLines[i];
	  double slope = (l[3] - l[1]) / (l[2] - l[0]);
	  for( size_t j = 0; j < linesFound; j++){
			if(abs(slope - lines[j].slope) < 0.05){
				continue;
			}
	  }
  }
  

  for( size_t i = 0; i < tempLines.size(); i++ )
  {
    Vec4i l = tempLines[i];
    line( ht, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,i * 50,0), 3, CV_AA);
  }
  


	imshow("Test", ht);
	waitKey();
	*/
}

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
	/*
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
  */
  //convert lines to vectors
  // consolidate? picture seems to be aliasing

  //check for intersections of perpendicular vectors (find 4!)

  //use getPerspectiveTransform()....done!
	/*
	imshow(windowName, n);


	waitKey();
	*/



