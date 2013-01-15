#include "Camera.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

cameraPerspective myObj;

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
    *r = (Point2f) (o1 + d1 * t1);
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
				if(found >=3)
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

cameraPerspective Camera::tryRotation(void){

	VideoCapture cap;
	cameraPerspective nullCP;
	Mat cam, grey, gat, ht, dht, out, gauss, can, newMat;
	int thresh = 1;
	Point2f corners[4];
	Point2f cornerSquare[4] = {Point2f(0,0),Point2f(0,480),Point2f(480,480),Point2f(480,0)};
	int found = 0;

	try{
	cap.open(0);
	}
	catch(Exception e){
		return nullCP;
	}

	cap>>cam;
	ht = cam;

	cvtColor( cam, grey, CV_RGB2GRAY);

	GaussianBlur( grey, gauss, Size( 3, 3 ), 0, 0 );

	Canny(gauss, can, 10, 250, 3);

	vector<vector<Point> > contours;
	findContours(can, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	RotatedRect box = minAreaRect(contours[0]);

	Mat rot_mat = getRotationMatrix2D(box.center,box.angle,1);

	myObj.angle = box.angle;
	myObj.center = box.center;

	Mat rotated;

	warpAffine(cam, rotated, rot_mat, cam.size(), INTER_CUBIC);


	cvtColor( rotated, grey, CV_RGB2GRAY);

	GaussianBlur( grey, gauss, Size( 3, 3 ), 0, 0 );

	Canny(gauss, can, 10, 250, 3);

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
		myObj.points[2*i] = pt1;
		myObj.points[(2*i)+1] = pt2;
		line(rotated,pt1,pt2,Scalar(255,0,0),1);
	}
	myObj.pic = rotated;

	cap.release();

	return myObj;
}

cameraPerspective Camera::findCorners(void){

	Point2f corners[4];
	int found = 0;
	Point2f cornerSquare[4] = {Point2f(0,0),Point2f(0,480),Point2f(480,480),Point2f(480,0)};

	for(int i = 0; i < 8; i +=2){
		for(int j = i + 2; j < 8; j+=2){
			if(intersection(myObj.points[i], myObj.points[i+1], myObj.points[j], myObj.points[j + 1], &(corners[found])) == true){
				if((corners[found].x > 900) || (corners[found].x < 0) || (corners[found].y > 900) || (corners[found].y < 0))
					continue;
				found++;
				if(found >= 4)
					break;
			}
		}
		if(found >= 4)
			break;
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
		circle(myObj.pic,corners[i],5,Scalar(255,0,0),3);
	}


	Mat homey, nullMat;

	homey = getPerspectiveTransform(corners,cornerSquare);

	warpPerspective(myObj.pic,nullMat,homey,myObj.pic.size());

	myObj.pic = nullMat;
	myObj.perspectiveWarp = homey;

	return myObj;
}

Point2f Camera::findPoint(Mat bg, cameraPerspective cp){
	return Point2f(0.0,0.0);
}

cameraPerspective Camera::getBackground(cameraPerspective cp){
	VideoCapture nCam;
	Mat m, rot, warp;

	try{
	nCam.open(0);
	}
	catch(Exception e){
		return cp;
	}

	nCam>>m;

	Mat rot_mat = getRotationMatrix2D(cp.center,cp.angle,1);
	warpAffine(m, rot, rot_mat, m.size(), INTER_CUBIC);
	warpPerspective(rot,warp,myObj.perspectiveWarp,rot.size());

	cp.background = warp.clone();
	return cp;
}


RotatedRect Camera::extractPoint(cameraPerspective cp){
	Mat bg, grey, thresh;
	vector<Vec4i> hierarchy;
	vector<vector<Point> > contour;
	cameraPerspective test;
	RotatedRect myRect;
	test = getBackground(cp);
	cvtColor( test.background, grey, CV_RGB2GRAY );
	threshold( grey, thresh, 130.0, 255.0, 0);
	findContours( thresh, contour, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	if(contour.size() >= 1){
		myRect = fitEllipse(Mat(contour[0]));
	}
	return myRect;
}

