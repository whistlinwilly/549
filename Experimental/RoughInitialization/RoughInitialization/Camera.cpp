#include "Camera.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;

Camera::Camera(){
}

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

Mat Camera::init(void){

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
		return Mat();
	}

	cap>>cam;
	ht = cam;

	imwrite("Orig0.jpg", cam);
//	waitKey();

	cvtColor( cam, grey, CV_RGB2GRAY);

	imwrite("Orig1_grey.jpg", grey);
//	waitKey();

	GaussianBlur( grey, gauss, Size( 3, 3 ), 0, 0 );
	imwrite("Orig2_gauss.jpg", gauss);
//	waitKey();

	Canny(gauss, can, 10, 250, 3);
	imwrite("Orig3_can.jpg", can);
//	waitKey();

	vector<vector<Point> > contours;
	findContours(can, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	RotatedRect box = minAreaRect(contours[0]);

	Mat rot_mat = getRotationMatrix2D(box.center,box.angle,1);

	Mat rotated;

	warpAffine(cam, rotated, rot_mat, cam.size(), INTER_CUBIC);

  imwrite("Rot0.jpg", rotated);
//	waitKey();

	cvtColor( rotated, grey, CV_RGB2GRAY);

	imwrite("Rot1_grey.jpg", grey);
//	waitKey();

	GaussianBlur( grey, gauss, Size( 3, 3 ), 0, 0 );
	imwrite("Rot2_gauss.jpg", gauss);
//	waitKey();

	Canny(gauss, can, 10, 250, 3);
	imwrite("Rot3_can.jpg", can);
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
				if(found >=4)
					break;
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
//	imshow("Test", rotated);
//	waitKey();
	}


	Mat homey, nullMat;

	homey = getPerspectiveTransform(corners,cornerSquare);

	warpPerspective(rotated,out,homey,rotated.size());

	imwrite("Rot4_final.jpg", out);


}

