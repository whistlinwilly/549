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


using namespace cv;
using namespace std;
int main(int argc,char** argv){
  Mat img;	
  img = imread(argv[1],1);
  int row = img.rows;
  int col = img.cols;
  Rect ROI((col-64)/2, (row-128)/2,64,128);
  Mat imageROI(img,ROI);
  //cout << "Here" << endl;
  char name[1024] ;
  strcpy(name,"");
  strcat(name,argv[1]);
  imwrite(name,imageROI);
  //cout << "ok" << endl;
  return 42;
}
