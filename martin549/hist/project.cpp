#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <cv.hpp>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <cassert>
#include <fstream>


#include <vector>
#include <cv.h>
#include <highgui.h>
#include <opencv2/contrib/contrib.hpp>


#define PI 3.14159265

using namespace cv;
using namespace std;
/****************************************/

/**************Image_info****************/

void image_info(Mat in){
  cout << "Reading Image Info..." << endl;
  int in_channel = in.channels();
  int in_rows = in.rows;
  int in_cols = in.cols;
  cout << endl << "in_channel = " << in_channel << endl;
  cout << endl << "in_rows = " << in_rows << endl;
  cout << endl << "in_cols = " << in_cols << endl;
  //cout << endl << "in = " << in << endl;
  return;
  
}

/***Generates Image for orient visualization***/
void complete_visual(int visual_index,Mat orient_visual,Mat norm_cell,int bin_size,int cell_size,int cols){
  int j;
  float max = norm_cell.at<float>(0,0); 
  float temp = max;
  int orient_index=0;
  for (j=1;j<bin_size;j++){
    temp = norm_cell.at<float>(0,j); 
    if (temp >= max){
      orient_index=j;
      max = temp;
    }
  }

  int x = (visual_index%cols)*cell_size+cell_size/2;
  int y = (visual_index/cols)*cell_size+cell_size/2;
  double delta = tan(orient_index*20*PI/180);
  line(orient_visual,Point((x-(cell_size/4)),(y+(cell_size/4)*delta)),Point((x+(cell_size/4)),(y-(cell_size/4)*delta)),cvScalar(255,0,0));
return;
}
/**********************************************/



/********Calculating Histograms*********/
void calc_histogram(int histogram[],Mat magnitude,Mat orientation,int x,int y,int cell_size,int bin_size){    
  int i,j;
  int cord_x,cord_y;
  int mag, orient;
  for (i=0;i<bin_size;i++){
    histogram[i]=0;
  }
  for (i=0;i<cell_size;i++){
    cord_y = y+i;
    for (j=0;j<cell_size;j++){
      cord_x = x+j;
      mag = magnitude.at<float>(cord_y,cord_x);
      orient = orientation.at<float>(cord_y,cord_x);
      if (orient>=0 && orient<20){
	histogram[0]+=mag;
      }
      else if (orient>=20 && orient<40){
	histogram[1]+=mag;
      }
      else if (orient>=40 && orient<60){
	histogram[2]+=mag;
      }
      else if (orient>=60 && orient<80){
	histogram[3]+=mag;
      }
      else if (orient>=80 && orient<100){
	histogram[4]+=mag;
      }
      else if (orient>=100 && orient<120){
	histogram[5]+=mag;
      }
      else if (orient>=120 && orient<140){
	histogram[6]+=mag;
      }
      else if (orient>=140 && orient<160){
	histogram[7]+=mag;
      }
      else{
	histogram[8]+=mag;
      }
    }
  }
  return;
}
/**************************************************/

  
/****Normalizing blocks****/
//Blocks are Mat format with size (4*(block_size)^2 * bin_size);
//Norm cells are bin_size row vector (int array);
void normalize_Block(Mat curr_cell,Mat norm_cell,Mat block,int bin_size,int block_size){
  int i,j;
  int index=-1;
  int norm=0;
  
  float temp1,temp2,temp3,temp4;

  float e = 1.1;

  float N1=0;
  float n1_1=0;
  float n1_2=0;
  float n1_3=0;
  float n1_4=0;

  float N2=0;
  float n2_1=0;
  float n2_2=0;
  float n2_3=0;
  float n2_4=0;

  float N3=0;
  float n3_1=0;
  float n3_2=0;
  float n3_3=0;
  float n3_4=0;

  float N4=0;
  float n4_1=0;
  float n4_2=0;
  float n4_3=0;
  float n4_4=0;

  /***Caculating N1***/
  for (i=0;i<block_size*block_size;i++){
    for (j=0;j<bin_size;j++){
      if (i==0){
	temp1 = block.at<float>(i,j);
	n1_1 += temp1*temp1;
	if (j==bin_size-1){
	  n1_1=sqrt(n1_1);
	}
      }
      if (i==1){
	temp2 = block.at<float>(i,j);
	n1_2 += temp2*temp2;
	if (j==bin_size-1){
	  n1_2=sqrt(n1_2);
	}
      }
      if (i==2){
	temp3 = block.at<float>(i,j);
	n1_3 += temp3*temp3;
	if (j==bin_size-1){
	  n1_3=sqrt(n1_3);
	}
      }
      if (i==3){
	temp4 = block.at<float>(i,j);
	n1_4 += temp4*temp4;
	if (j==bin_size-1){
	  n1_4=sqrt(n1_4);
	}
      }
    }
  }
  N1 = sqrt(n1_1*n1_1+n1_2*n1_2+n1_3*n1_3+n1_4*n1_4+e) ;
      
  /***Caculating N2***/
  for (i=block_size*block_size;i<2*block_size*block_size;i++){
    for (j=0;j<bin_size;j++){
      if (i==block_size*block_size){
	temp1 = block.at<float>(i,j);
	n2_1 += temp1*temp1;
	if (j==bin_size-1){
	  n2_1=sqrt(n2_1);
	}
      }
      if (i==block_size*block_size+1){
	temp2 = block.at<float>(i,j);
	n2_2 += temp2*temp2;
	if (j==bin_size-1){
	  n2_2=sqrt(n2_2);
	}
      }
      if (i==block_size*block_size+2){
	temp3 = block.at<float>(i,j);
	n2_3 += temp3*temp3;
	if (j==bin_size-1){
	  n2_3=sqrt(n2_3);
	}
      }
      if (i==block_size*block_size+3){
	temp4 = block.at<float>(i,j);
	n2_4 += temp4*temp4;
	if (j==bin_size-1){
	  n2_4=sqrt(n2_4);
	}
      }
    }
  }
  N2 = sqrt(n2_1*n2_1+n2_2*n2_2+n2_3*n2_3+n2_4*n2_4+e) ;

  /***Caculating N3***/
  for (i=2*block_size*block_size;i<3*block_size*block_size;i++){
    for (j=0;j<bin_size;j++){
      if (i==2*block_size*block_size){
	temp1 = block.at<float>(i,j);
	n3_1 += temp1*temp1;
	if (j==bin_size-1){
	  n3_1=sqrt(n3_1);
	}
      }
      if (i==2*block_size*block_size+1){
	temp2 = block.at<float>(i,j);
	n3_2 += temp2*temp2;
	if (j==bin_size-1){
	  n3_2=sqrt(n3_2);
	}
      }
      if (i==2*block_size*block_size+2){
	temp3 = block.at<float>(i,j);
	n3_3 += temp3*temp3;
	if (j==bin_size-1){
	  n3_3=sqrt(n3_3);
	}
      }
      if (i==2*block_size*block_size+3){
	temp4 = block.at<float>(i,j);
	n3_4 += temp4*temp4;
	if (j==bin_size-1){
	  n3_4=sqrt(n3_4);
	}
      }
    }
  }
  N3 = sqrt(n3_1*n3_1+n3_2*n3_2+n3_3*n3_3+n3_4*n3_4+e) ;
  

  /***Caculating N4***/
  for (i=3*block_size*block_size;i<4*block_size*block_size;i++){
    for (j=0;j<bin_size;j++){
      if (i==3*block_size*block_size){
	temp1 = block.at<float>(i,j);
	n4_1 += temp1*temp1;
	if (j==bin_size-1){
	  n4_1=sqrt(n4_1);
	}
      }
      if (i==3*block_size*block_size+1){
	temp2 = block.at<float>(i,j);
	n4_2 += temp2*temp2;
	if (j==bin_size-1){
	  n4_2=sqrt(n4_2);
	}
      }
      if (i==3*block_size*block_size+2){
	temp3 = block.at<float>(i,j);
	n4_3 += temp3*temp3;
	if (j==bin_size-1){
	  n4_3=sqrt(n4_3);
	}
      }
      if (i==3*block_size*block_size+3){
	temp4 = block.at<float>(i,j);
	n4_4 += temp4*temp4;
	if (j==bin_size-1){
	  n4_4=sqrt(n4_4);
	}
      }
    }
  }
  N4 = sqrt(n4_1*n4_1+n4_2*n4_2+n4_3*n4_3+n4_4*n4_4+e);

  for (i=0;i<bin_size;i++){
    float value = curr_cell.at<float>(0,i);
    norm_cell.at<float>(0,i) = value/N1;
    norm_cell.at<float>(1,i) = value/N2;
    norm_cell.at<float>(2,i) = value/N3;
    norm_cell.at<float>(3,i) = value/N4;
  }
  
  return;
}


vector<Point2f> lineToPointPair(Vec4f line){
    vector<Point2f> points;

    points.push_back(Point2f(line[0], line[1]));
    points.push_back(Point2f(line[2], line[3]));

    return points;
}


// from wiki
Point2f computeIntersect(Vec4f line1, Vec4f line2){
    vector<Point2f> p1 = lineToPointPair(line1);
    vector<Point2f> p2 = lineToPointPair(line2);

    float denom = (p1[0].x - p1[1].x)*(p2[0].y - p2[1].y) - (p1[0].y - p1[1].y)*(p2[0].x - p2[1].x);
    Point2f intersect(((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].x - p2[1].x) -
                       (p1[0].x - p1[1].x)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom,
                      ((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].y - p2[1].y) -
                       (p1[0].y - p1[1].y)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom);

    return intersect;
}



bool acceptLinePair(Vec2f line1, Vec2f line2, float minTheta)
{
    float theta1 = line1[1], theta2 = line2[1];

    if(theta1 < minTheta)
    {
        theta1 += CV_PI; // dealing with 0 and 180 ambiguities...
    }

    if(theta2 < minTheta)
    {
        theta2 += CV_PI; // dealing with 0 and 180 ambiguities...
    }

    return abs(theta1 - theta2) > minTheta;
}


bool closeLinesPair(Vec4f l1, Vec4f l2, float error) {
  float d1 = abs(l1[0] - l2[0]);
  float d2 = abs(l1[1] - l2[1]);
  float d3 = abs(l1[2] - l2[2]);
  float d4 = abs(l1[3] - l2[3]);
  if (d1 + d2 + d3 + d4 < (4.0 * error)) {
    return true;
  } else { return false; }
}


void drawSquares( Mat& image, const vector<vector<Point> >& squares ){
  for( size_t i = 0; i < squares.size(); i++ )
  {
    const Point* p = &squares[i][0];
    int n = (int)squares[i].size();
    polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, CV_AA);
  }
  imshow("test", image);
}

double angle( Point pt1, Point pt2, Point pt0 ){
  double dx1 = pt1.x - pt0.x;
  double dy1 = pt1.y - pt0.y;
  double dx2 = pt2.x - pt0.x;
  double dy2 = pt2.y - pt0.y;
  return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}



bool byX(Point2f pt1, Point2f pt2) {
  if (pt1.x <= pt2.x) return true;
  else return false;
}

bool byY(Point2f pt1, Point2f pt2) {
  if (pt1.y <= pt2.y) return true;
  else return false;
}



float getMedianX (vector<Point2f> pts) {
  sort(pts.begin(), pts.end(), byX);
  float medianX = pts[pts.size()/2].x;
  return medianX;
}

float getMedianY (vector<Point2f> pts) {
  sort(pts.begin(), pts.end(), byY);
  float medianY = pts[pts.size()/2].y;
  return medianY;
}

Point2f approxCorner(vector<Point2f> pts) {
  float medianX = getMedianX(pts);
  float medianY = getMedianY(pts);
//  cout << "medianX is " << medianX << endl;
//  cout << "medianY is " << medianY << endl;
  float error = 10.0, sumX = 0.0, sumY = 0.0;
  int count = 0;

  for (int i = 0; i < pts.size(); i ++) {
    Point2f pt = pts[i];
 //   cout << pt.x << " : " << pt.y << endl;
    float dx = abs(pt.x - medianX);
    float dy = abs(pt.y - medianY);
 //   cout << "dx is " << dx << " dy is " << dy << endl; 
    if (sqrt(pow(dx,2) + pow(dy, 2)) < error) {
      sumX += pt.x;
      sumY += pt.y;
      count ++;
 //     cout << "yes !!!" << endl;
    }
  }
 // cout << "count is " << count << endl;
 // cout << "return value is " << sumX/count << " : " << sumY/count << endl;
  return Point2f(sumX/count, sumY/count);
}

















int main(int argc, char** argv){

  Mat image;
  Mat gray, gray0, test0, test1, test2, test3, test4, test5;


  if (argc < 2){
    cout << "Give me an image" << endl;
    return 42;
  }

  if (image.data){
    cout << "No image data...try another pic" << endl;
    return 42;
  }

  image = imread(argv[1],1);
  Mat copy = image;
  int width = image.cols; 
  int height = image.rows;

  cout << "width is " << width << " height is " << height << endl;
  



//  GaussianBlur(image,image,Size(3,3),0,0,BORDER_DEFAULT);
//  blur(image, image, Size(3,3));
  
  // split into r, g, b single channel
  vector<Mat> bgr_planes;
  split( image, bgr_planes );

  equalizeHist(bgr_planes[0], bgr_planes[0]);
  equalizeHist(bgr_planes[1], bgr_planes[1]);
  equalizeHist(bgr_planes[2], bgr_planes[2]);

  merge(bgr_planes, test0);
  cvtColor(test0, gray, CV_RGB2GRAY);
  imwrite("merge-gray.png",gray);

  int numOfHoughLines = 1000;
  vector<Vec4i> lines;
//  vector<Vec2f> lines;
  while (numOfHoughLines >= 20) {
    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat hist;
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    int total = image.rows * image.cols;
    int halfTotal = total / 2;
  //  cout << "total count is " << total << endl;
    float current = 0;
    for (int i = 0; i < histSize; i ++) {
      current += hist.at<float>(i);
      if (current >= halfTotal) {
        current = i;
        break;
      }
    }

    Canny(gray, test5, 0.66*current, 1.33*current, 3, true);
    imwrite("cannyMedian.png", test5);

    int houghLinesPThreshold = 100;
    int minLineLength = 10;
    int maxLineGap = 10;
  
 //   HoughLines(test5, lines, 1, CV_PI/180, houghLinesPThreshold, minLineLength, maxLineGap);
    HoughLinesP(test5, lines, 3, CV_PI/180, houghLinesPThreshold, minLineLength, maxLineGap);

    cout << "number of lines detected is " << lines.size() << endl;

    numOfHoughLines = lines.size();
    GaussianBlur(gray,gray,Size(3,3),0,0,BORDER_DEFAULT);
  }








  for(int i = 0; i < lines.size(); i++){
    Vec4i l = lines[i];
    line(copy, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
  }
//  line(copy, Point(-1, -1), Point(2000, 2000), Scalar(255,0,0), 1, CV_AA);
  imwrite("detectLines.png", copy);




  vector<Vec4d> extendLines;
  vector<float> angleLines; //angle is in the unit of degree
  for (int i = 0; i < lines.size(); i++){
    Vec4i l = lines[i];
    float x1 = 1.0*l[0], y1 = 1.0*l[1], x2 = 1.0*l[2], y2 = 1.0*l[3];
    Vec4f temp;
    float angle = (180.0/PI)*atan2((y2-y1),(x2-x1));
  //  cout << "at i = " << i << " angle is " << angle << " and " << tan(angle/180.0*CV_PI) << endl;
    float a = cos(angle/180.0*CV_PI), b = sin(angle/180.0*CV_PI);
    temp[0] = x1 - 10000*a;
    temp[1] = y1 - 10000*b;
    temp[2] = x1 + 10000*a;
    temp[3] = y1 + 10000*b;
 /*
    pt1.x = cvRound(x0 + 1000*(-b));
    pt1.y = cvRound(y0 + 1000*(a));
    pt2.x = cvRound(x0 - 1000*(-b));
    pt2.y = cvRound(y0 - 1000*(a));

    float xa = 0.0, ya = tan(angle) * (xa - l[0]) + l[1];
    float yb = 0.0, xb = (yb - l[1])/ tan(angle) + l[0];
//    cout << "at i = " << i << " " << endl;
    if (ya >= 0.0) {
      temp[0] = xa;
      temp[1] = ya;
//      cout << "x1 " << xa << " y1 " << ya << "   " << endl;
    } else {
      temp[0] = xb;
      temp[1] = yb;
//      cout << "x1 " << xb << " y1 " << yb << "   " << endl;
    }
    float xc = width, yc = tan(angle) * (xc - l[0]) + l[1];
    float yd = height, xd = (yd - l[1])/ tan(angle) + l[0];
    if (yc < height) {
      temp[2] = xc;
      temp[3] = yc;
//      cout << "x2 " << xc << " y2 " << yc << "   " << endl;
    } else {
      temp[2] = xd;
      temp[3] = yd;
//      cout << "x2 " << xd << " y2 " << yd << "   " << endl;
    }
    */
    extendLines.push_back(temp);
    angleLines.push_back(angle);
  }
  cout << "number of extended lines: " << extendLines.size() << endl;




/*
  for (int i = 0; i < extendLines.size(); i ++) {
    Vec4f l = extendLines[i];
    line(copy, Point2f(l[0], l[1]), Point2f(l[2], l[3]), Scalar(0,255,0), 1, CV_AA);
  
  }

  imwrite("extendLines.png", copy);

*/












  vector<Point2f> intersections;
  for(size_t i = 0; i < extendLines.size(); i++ ){
    for(size_t j = 0; j < extendLines.size(); j++){
      Vec4f line1 = extendLines[i];
      Vec4f line2 = extendLines[j];
      if(abs(abs(angleLines[i]) - abs(angleLines[j])) > 30){
  //      cout << "angle i is " << angleLines[i] << endl;
  //      cout << "angle j is " << angleLines[j] << endl;
        Point2f intersection = computeIntersect(line1, line2);
        intersections.push_back(intersection);
      }
    }
  }

  vector<Point2f> topLeft, topRight, bottomLeft, bottomRight;
  for(vector<Point2f>::iterator i = intersections.begin(); i != intersections.end(); ++i){
    Point2f pt = *i;
    float x = pt.x, y = pt.y;
    if (x < 0.25*width && y < 0.25*height){
      topLeft.push_back(pt);
    } else if (x < 0.25*width && y > 0.75*height) {
      bottomLeft.push_back(pt);
    } else if (x > 0.75*width && y < 0.25*height) {
      topRight.push_back(pt);
    } else if (x > 0.75*width && y > 0.75*height) {
      bottomRight.push_back(pt);
    } else {
      continue;
    }
  }
  cout << "total intersections " << intersections.size() << endl;
  cout << "topLeft " << topLeft.size() << endl;
  cout << "topRight " << topRight.size() << endl;
  cout << "bottomLeft " << bottomLeft.size() << endl;
  cout << "bottomRight " << bottomRight.size() << endl;


  cout << "get top left" << endl;
  Point2f tl = approxCorner(topLeft);

  cout << "get top right" << endl;
  Point2f tr = approxCorner(topRight);

  cout << "get bottom left" << endl;
  Point2f bl = approxCorner(bottomLeft);

  cout << "get bottom right" << endl;
  Point2f br = approxCorner(bottomRight);


/*
  if(intersections.size() > 0) {
    vector<Point2f>::iterator i;
    for(i = intersections.begin(); i != intersections.end(); ++i){
  //       cout << "Intersection is " << i->x << ", " << i->y << endl;
      circle(copy, *i, 1, Scalar(0, 255, 0), 3);
    }
  }
*/

  
  circle(copy, tl, 1, Scalar(255, 0, 0), 3);
  circle(copy, tr, 1, Scalar(255, 0, 0), 3);
  circle(copy, bl, 1, Scalar(255, 0, 0), 3);
  circle(copy, br, 1, Scalar(255, 0, 0), 3);

 // circle(copy, Point2f(589.0, 466.518), 1, Scalar(0, 0, 255), 3);



  imwrite("intersectPoints.jpg", copy);



  line(copy, tl, tr, Scalar(0, 255, 0), 2);
  line(copy, tl, bl, Scalar(0, 255, 0), 2);
  line(copy, bl, br, Scalar(0, 255, 0), 2);
  line(copy, tr, br, Scalar(0, 255, 0), 2);

  imwrite("boundingRect.jpg", copy);














  
/*
// find contours and store them all as a list
  vector<vector<Point> > contours;
  vector<vector<Point> > squares;
  findContours(test4, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
  cout << "contours size is " << contours.size() << endl;
  vector<Point> approx;
  for( size_t i = 0; i < contours.size(); i++ ){
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
    cout << "area for i = " << i << " is " << fabs(contourArea(Mat(approx))) << endl;
    if( approx.size() == 4 &&
      fabs(contourArea(Mat(approx))) > 10000 &&
      isContourConvex(Mat(approx)) ){

        squares.push_back(approx);
    }
  }

  cout << "squares size is " << squares.size() << endl;
  for (int i = 0; i < squares.size(); i ++) {
    const Point* p = &squares[i][0];
    int n = (int)squares[i].size();
    polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, CV_AA);
 //   vector<Point> pts = squares[i];
 //   cout << "points " << pts[0] << pts[1] << pts[2] << pts[3] << endl;
  }
  imwrite("test6.png", copy);
*/



/*
   vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
findContours( test5, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  //  findContours( test5, contours, hierarchy,
  //      CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        Scalar color( rand()&255, rand()&255, rand()&255 );
        drawContours( test4, contours, idx, color, CV_FILLED, 8, hierarchy );
    }


  
  */


















/*
  Mat abs_grad_x, abs_grad_y;
  Mat gradient_mag,gradient_orient;


 
  int ddepth = CV_16S;
  int scale = 1;
  int delta = 0;
  int ksize = 1;  // [-1,0,1] mask

  
  int i,j,k;
  int grad_x_mag, grad_y_mag;
  


  Mat grad_x = cvCreateMat(gray.rows,gray.cols,CV_16S);
  Sobel(gray,grad_x,ddepth,1,0,ksize,scale,delta,BORDER_DEFAULT); 
  convertScaleAbs( grad_x, abs_grad_x );

  Mat grad_y = cvCreateMat(gray.rows,gray.cols,CV_16S);
  Sobel(gray,grad_y,ddepth,0,1,ksize,scale,delta,BORDER_DEFAULT);
  convertScaleAbs(grad_y,abs_grad_y);

  gradient_mag = Mat::zeros(gray.rows,gray.cols,CV_32F);
  accumulateSquare(abs_grad_x,gradient_mag);
  accumulateSquare(abs_grad_y,gradient_mag);
  sqrt(gradient_mag,gradient_mag);  
  imwrite("gradientX.png",abs_grad_x);
  imwrite("gradientY.png",abs_grad_y);
  imwrite("gradientMag.png",gradient_mag);



*/










  /*
  float grad_orient_data;
  int index;
  gradient_orient = Mat::zeros(gray.rows,gray.cols,CV_32F);
  for (i=0;i<gray.rows;i++){
    for (j=0;j<gray.cols;j++){
      grad_x_mag = grad_x.at<short>(i,j);
      grad_y_mag = grad_y.at<short>(i,j);   
	if (grad_x_mag == 0){
	  grad_orient_data = 90;	  
	}
	else{
	  grad_orient_data =(180.0/PI)*(atan2(grad_y_mag,grad_x_mag));
	  if (grad_orient_data<=0){
	    grad_orient_data+=180;
	  }	  
	}	
	gradient_orient.at<float>(i,j) = grad_orient_data;                   
    }
  }
  */


  /***Generating histograms for each cell***/
  /***Storing it by rows in Mat pic***/
  /*
  int cell_size = 8;
  int bin_size = 9;
  int block_size = 2;
  int cell_rows = gray.rows/cell_size;
  int cell_cols = gray.cols/cell_size;
  int cell_num = cell_rows*cell_cols;
  int histogram[bin_size];
  Mat curr_cell = Mat::zeros(1,bin_size,CV_32F);
  Mat norm_cell = Mat::zeros(block_size*block_size,bin_size,CV_32F); 
  Mat pic = Mat::zeros(cell_num,bin_size,CV_32F); 
  Mat block = Mat::zeros(4*(block_size*block_size),bin_size,CV_32F);
  Mat orient_visual = Mat::zeros(gray.rows-cell_size,gray.cols-cell_size,CV_32F);
  
  Mat HOG = Mat::zeros((cell_rows-2)*(cell_cols-2),4*bin_size,CV_32F);
  int up_left_cord_x, up_left_cord_y;
  int pic_x=0;
  int pic_y;
  int pic_index;
  int block_row_count = 0;
  int HOG_index = -1;
  int m,n;
  float HOG_value;
  */

  return 42;
}
