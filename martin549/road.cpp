#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

#define PI 3.14159265

using namespace cv;
using namespace std;

void add_count(Mat counter_matrix,float x1,float y1,float angle,int cell_size,int row_num,int col_num){
  int row_index = y1/cell_size;
  int col_index = x1/cell_size;
  int cell_index = row_index*col_num+col_index;
  int angle_index = (angle+180)/10;
  float count = counter_matrix.at<float>(cell_index,angle_index)+1;
  counter_matrix.at<float>(cell_index,angle_index) = count;
  //cout << "added 1 at " << cell_index << ", " << angle_index << endl;
  //cout << "count = " << count << endl;
  return;
}



int main(int argc, char** argv){
  Mat image = imread(argv[1],1);
  //imshow("original image",image);
  Mat image_gray,result,result_color;
  int threshold_value = 200;
  int threshold_type = 0; /*binary inverted*/
  int max_binary_value = 255;
  cvtColor(image,image_gray,CV_RGB2GRAY);
  int i,j;
  int sky = image_gray.rows/3;

  //imshow("gray scale",image_gray);
  //cout << image_gray.rows << " * " << image_gray.cols;
  for (i=0;i<image_gray.cols;i++){
    for (j=0;j<sky;j++){
      image_gray.at<int>(j,i) = 0;
    }
  }
  threshold(image_gray,result,threshold_value,max_binary_value,threshold_type);
  imwrite("/home/xinwu/WORK/binary_map.png",result);
  imshow("binary map",result);
  cvtColor(result,result_color,CV_GRAY2BGR);
  int line_count=0;
  
  /*
  vector<Vec2f> lines;
  //vector<Point2f> pt_lines;
  HoughLines(result, lines, 1, CV_PI/180, 200);

  for( size_t i = 0; i < lines.size(); i++ )
  {
    line_count++;
    float rho = lines[i][0], theta = lines[i][1];
    Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    pt1.x = cvRound(x0 + 1000*(-b));
    pt1.y = cvRound(y0 + 1000*(a));
    pt2.x = cvRound(x0 - 1000*(-b));
    pt2.y = cvRound(y0 - 1000*(a));
    //pt_lines(i)[0] = pt1;
    //pt_lines(i)[1] = pt2;
    line(result_color, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
  }
  */
  
  int houghLinesPThreshold = 100;
  int minLineLength = 200;
  int maxLineGap = 50;
  vector<Vec4i> lines;
  HoughLinesP(result,lines,1,CV_PI/180,houghLinesPThreshold,minLineLength,maxLineGap);
  int cell_size = 60;
  int row_num = result.rows/cell_size;
  int col_num = result.cols/cell_size;
  int cell_num = row_num*col_num;
  int bin_size =36;
  //cout << row_num << " * " << col_num << endl;
  

  cout << row_num << endl;
  cout << col_num << endl;
  cout << result.rows << " * " << result.cols << endl;
  cout << cell_num << endl;
  float angle,x1,x2,y1,y2;
  Mat counter_matrix = Mat::zeros(cell_num,bin_size,CV_32F);
  for (size_t i = 0; i < lines.size(); i++ ){
    
    x1 = lines[i][0];
    x2 = lines[i][2];   
    y1 = lines[i][1];
    y2 = lines[i][3];
    angle = (180.0/PI)*atan2((y1-y2),(x1-x2));

    //cout << "x1: " << x1 << " y1: " << y1 << endl;
    //cout << "x2: " << x2 << " y2: " << y2 << endl;
    //cout << "angle: " << angle << endl;
    if (y1>y2)
      add_count(counter_matrix,x1,y1,angle,cell_size,row_num,col_num);
    else
      add_count(counter_matrix,x2,y2,angle,cell_size,row_num,col_num);
  }
  //cout << counter_matrix << endl;

  float k;
  int row_index;
  int col_index;
  /*Displaying line segments*/
  for( i = 0; i < lines.size(); i++ ){
      line_count++;
      Vec4i l = lines[i];
      //cout << "x1: " << l[0] << " y1: " << l[1] << endl;
      //cout << "x2: " << l[2] << " y2: " << l[3] << endl;

      x1 = lines[i][0];
      x2 = lines[i][2];   
      y1 = lines[i][1];
      y2 = lines[i][3];
      angle = (180.0/PI)*atan2((y1-y2),(x1-x2));

      if (y1>y2){
	row_index = y1/cell_size;
	col_index = x1/cell_size;
      }
      else{
	row_index = y2/cell_size;
	col_index = x2/cell_size;
      }
      int cell_index = row_index*col_num+col_index;
      int angle_index = (angle+180)/10;
      float count = counter_matrix.at<float>(cell_index,angle_index);         
      if ((abs(l[0]- l[2]) >= 20) && (abs(l[1]-l[3]) >= 20))/*get rid of vertical lines*/{
	if (count >= 2){
	  k++;
	  cout << cell_index << " and " << angle_index << endl;
	  line( result_color, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
	}
      }
   }
  
  Mat mid_lane =  Mat::zeros(2,col_num,CV_32F);
  int mid_lane_count = 0;
  int length = 500;
  //pick out the right lane
  //cout << counter_matrix << endl;
  for (i=cell_num-col_num;i<cell_num;i++){
    for (int j = 0;j<bin_size;j++){
    //cout << "here" << endl;
    float count = counter_matrix.at<float>(i,j);
    if (j<18)
      angle = (j*10+5)*PI/180;
    else
      angle = (j*10-180+5)*PI/180;
    if (count >=2){
      mid_lane.at<float>(0,mid_lane_count) = i;
      mid_lane.at<float>(1,mid_lane_count) = angle;
      mid_lane_count++;
      cout << count << endl;
      cout << i << " " << j << endl;
      cout << i%col_num*cell_size << endl;
      cout << i/row_num*cell_size << endl;
    }
    }
  }
  cout << "here it commmmmmmmmes" << endl;
  cout << mid_lane << endl;
  int mid_index = cell_num-col_num/2;
  for (i=0;i<col_num;i++){
    int temp = mid_lane.at<float>(0,i)-mid_index;
    mid_lane.at<float>(0,i) = temp;
  }
  cout << "here it commmmmmmmmes again" << endl;
  cout << mid_lane << endl;
  
  int min_pos = mid_index;
  int max_neg = -mid_index;
  int min_pos_index,max_neg_index;

  for (i=0;i<col_num;i++){
    float temp = mid_lane.at<float>(0,i);
    if ((temp > 0) && (temp < min_pos)){
      min_pos_index = i;
      min_pos = temp;
    }
    if ((temp < 0) && (temp > max_neg)) {
      max_neg_index = i;
      max_neg = temp;
    }
  }


  cout << min_pos_index << endl;
  cout << max_neg_index << endl;
 
  Mat found =  Mat::zeros(2,2,CV_32F);
  found.at<float>(0,0) = mid_lane.at<float>(0,min_pos_index);
  found.at<float>(0,1) = mid_lane.at<float>(0,max_neg_index);
  found.at<float>(1,0) = mid_lane.at<float>(1,min_pos_index);
  found.at<float>(1,1) = mid_lane.at<float>(1,max_neg_index);  

  cout << "found" << found << endl;
    
  for (i=0;i<2;i++){
  
    int lane_index = found.at<float>(0,i)+mid_index;
    float angle = found.at<float>(1,i);
    int start_x = (lane_index%col_num+0.5)*cell_size;
    int start_y = (lane_index/col_num+0.5)*cell_size;
    float d_x = length*cos(angle);
    float d_y = abs(length*sin(angle));
    line(result_color,Point(start_x,start_y),Point(start_x-d_x,start_y-d_y),Scalar(0,255,0),1,CV_AA);
  }

   line( result_color, Point(120,0), Point(120,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(240,0), Point(240,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(360,0), Point(360,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(480,0), Point(480,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(600,0), Point(600,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(720,0), Point(720,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(840,0), Point(840,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(960,0), Point(960,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(1080,0), Point(1080,960), Scalar(255,0,0), 1, CV_AA);
  line( result_color, Point(1200,0), Point(1200,960), Scalar(255,0,0), 1, CV_AA);
  
  //cout << "k= " << k << endl;
  //cout << "total of " << line_count << " lines detected"<< endl;
  imshow("line segments",result_color);
  imwrite("/home/xinwu/WORK/line_seg.png",result_color);
  waitKey();
  return 0;
}
