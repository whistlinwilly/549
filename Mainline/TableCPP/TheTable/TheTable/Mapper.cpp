#include "Mapper.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\opencv.hpp>



Mapper::Mapper(){
	cDatSmall = new circleData();
	cDatBig = new circleData();

}
float Mapper::xToInches(float pixels){
	return ((pixels/TABLE_X)*TABLE_WIDTH);
}

float Mapper::yToInches(float pixels){
	return ((pixels/TABLE_Y)*TABLE_WIDTH);
}

float Mapper::xToPixels(float inches){
	return ((inches/TABLE_WIDTH)*TABLE_X);
}

float Mapper::yToPixels(float inches){
	return ((inches/TABLE_HEIGHT)*TABLE_Y);
}

float Mapper::toRads(float degrees){
	return ((degrees/360.0)*2*M_PI);
}

float Mapper::toDegrees(float radians){
	return ((radians/(2*M_PI))*360.0);
}

void Mapper::computeProjectorData(){
	float deltX;
	float deltY;
	float distFromCenter;
	float eyeDeltaX;
	float eyeDeltaY;

	if (cDatBig->c1w > cDatBig->c2w){
		isFirst = 1;
		if (cDatBig->c1x > cDatBig->c2x){
				rotationAdd = 180.00;
		}
		centerX = cDatBig->c1x;
		centerY = cDatBig->c1y;
		centerZ = 0;
		distanceFromTable = ((cDatBig->c1w) / 4.0 / TABLE_X * TABLE_WIDTH) * DEFAULT_DISTANCE;
		//subtract from 360 and subtract 90 to make rotation starting from +x
		rotation = 360.0 - (cDatBig->c1r + 90) + rotationAdd;
		if (rotation > 360.0) rotation -= 360.0;
		incidentAngle = 360.0*(asin(cDatBig->c1w/cDatBig->c1h)/(2*M_PI));
		eyeX = (centerX - (TABLE_X / 2.0)) / TABLE_X * TABLE_WIDTH;
		eyeY = ((TABLE_Y / 2.0) - centerY) / TABLE_Y * TABLE_HEIGHT;
		eyeZ = distanceFromTable*(sin(toRads(incidentAngle)));
		deltX = cos(toRads(rotation));
		deltY = sin(toRads(rotation));
		distFromCenter = distanceFromTable * cos(toRads(incidentAngle));
		eyeDeltaX = distFromCenter * deltX;
		eyeDeltaY = distFromCenter * deltY;
		centerX = eyeX;
		centerY = eyeY;
		eyeX = eyeX + eyeDeltaX;
		eyeY = eyeY + eyeDeltaY;

		if (cDatBig->c1y > cDatBig->c2y){
			twist = 90 - toDegrees(atan((cDatBig->c2x - cDatBig->c1x)/(cDatBig->c1y - cDatBig->c2y)));
			//twist = toDegrees(-atan((cDatBig->c2y - cDatBig->c1y)/(cDatBig->c2x - cDatBig->c1x)) - toRads(rotation));
		}
		else {
			twist = 270 - toDegrees(atan((cDatBig->c2x - cDatBig->c1x)/(cDatBig->c1y - cDatBig->c2y)));
		}
		twist -= 180.00;
		twist -= rotation;
		//twist = toDegrees(-atan((cDatBig->c1y - cDatBig->c2y)/(cDatBig->c1x - cDatBig->c2x)) - toRads(rotation));
		
	}
	else {
		isFirst = 0;
		if (cDatBig->c2x > cDatBig->c1x){
				rotationAdd = 180.00;
		}
		centerX = cDatBig->c2x;
		centerY = cDatBig->c2y;
		centerZ = 0;
		distanceFromTable = ((cDatBig->c2w) / 4.0 / TABLE_X * TABLE_WIDTH) * DEFAULT_DISTANCE;
		//subtract from 360 and subtract 90 to make rotation starting from +x
		rotation = 360.0 - (cDatBig->c2r + 90.0) + rotationAdd;
		if (rotation > 360.0) rotation -= 360.0;
		incidentAngle = 360*(asin(cDatBig->c2w/cDatBig->c2h)/(2*M_PI));
		eyeX = (centerX - (TABLE_X / 2.0)) / TABLE_X * TABLE_WIDTH;
		eyeY = ((TABLE_Y / 2.0) - centerY) / TABLE_Y * TABLE_HEIGHT;
		eyeZ = distanceFromTable*(sin(toRads(incidentAngle)));
		deltX = cos(toRads(rotation));
		deltY = sin(toRads(rotation));
		distFromCenter = distanceFromTable * cos(toRads(incidentAngle));
		eyeDeltaX = distFromCenter * deltX;
		eyeDeltaY = distFromCenter * deltY;
		centerX = eyeX;
		centerY = eyeY;
		eyeX = eyeX + eyeDeltaX;
		eyeY = eyeY + eyeDeltaY;

		if (cDatBig->c2y > cDatBig->c1y){
			twist = 90 - toDegrees(atan((cDatBig->c1x - cDatBig->c2x)/(cDatBig->c2y - cDatBig->c1y)));
			//twist = toDegrees(-atan((cDatBig->c2y - cDatBig->c1y)/(cDatBig->c2x - cDatBig->c1x)) - toRads(rotation));
		}
		else {
			twist = 270 - toDegrees(atan((cDatBig->c1x - cDatBig->c2x)/(cDatBig->c2y - cDatBig->c1y)));
		}
		twist -= 180.00;
		twist -= rotation;
	}

	//make rotation full 360 capable
		/*if (cDatBig->c1w > cDatBig->c2w){
			if (cDatBig->c1x > cDatBig->c2x){
				rotation += 180.00;
			}
			else twist += 180.00;
		}
		else {
			if (cDatBig->c2x > cDatBig->c1x){
				rotation += 180.00;
			}
			else twist += 180.00;
		}*/

}

void Mapper::computeUpVector(){

	//calculate up vector including twist
	upFromTwist();

	//build coordinate string to send
	buildString();
}

void Mapper::upFromTwist(){
	/*
	* this part of the code calculates the "up" vector for the opengl camera position
	* first we find the vector from the camera to the focal point in the table plane
	* by crossing this vector with a (0,0,1) vector rotated about the first vector by twist degrees
	* we obtain a vector pointing out the right side, perpendicular to the look at vector
	* finally we cross this third right side vector with the original "look at" vector to obtain one
	* perpendicular to them both - that is the "up" vector we need
	*/

	float vecX = centerX - eyeX;
	float vecY = centerY - eyeY;
	float vecZ = 0.0 - eyeZ;

	// normalized look at vector
	float newVecX = vecX / sqrt(pow(vecX, 2) + pow(vecY, 2) + pow(vecZ, 2));
	float newVecY = vecY / sqrt(pow(vecX, 2) + pow(vecY, 2) + pow(vecZ, 2));
	float newVecZ = vecZ / sqrt(pow(vecX, 2) + pow(vecY, 2) + pow(vecZ, 2));

	vecX = newVecX;
	vecY = newVecY;
	vecZ = newVecZ;

	// upwards z vector
	float zVecX = 0.0;
	float zVecY = 0.0;
	float zVecZ = 1.0;

	float cosTheta = (float)cos(toRads(-twist));
	float sinTheta = (float)sin(toRads(-twist));

	// x position of the new rotated vector
	float x   = (cosTheta + (1 - cosTheta) * vecX * vecX)		* zVecX;
	x  += ((1 - cosTheta) * vecX * vecY - vecZ * sinTheta)	* zVecY;
	x  += ((1 - cosTheta) * vecX * vecZ + vecY * sinTheta)	* zVecZ;

	// y position of the new rotated vector
	float y  = ((1 - cosTheta) * vecX * vecY + vecZ * sinTheta)	* zVecX;
	y += (cosTheta + (1 - cosTheta) * vecY * vecY)		* zVecY;
	y += ((1 - cosTheta) * vecY * vecZ - vecX * sinTheta)	* zVecZ;

	// z position of the new rotated vector
	float z  = ((1 - cosTheta) * vecX * vecZ - vecY * sinTheta)	* zVecX;
	z  += ((1 - cosTheta) * vecY * vecZ + vecX * sinTheta)	* zVecY;
	z  += (cosTheta + (1 - cosTheta) * vecZ * vecZ)		* zVecZ;

	newVecX = x / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	newVecY = y / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	newVecZ = z / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	x = newVecX;
	y = newVecY;
	z = newVecZ;

	//lookat X zUp

	float rightX = vecY*z - vecZ*y;
	float rightY = vecZ*x - vecX*z;
	float rightZ = vecX*y - vecY*x;

	newVecX = rightX / sqrt(pow(rightX, 2) + pow(rightY, 2) + pow(rightZ, 2));
	newVecY = rightY / sqrt(pow(rightX, 2) + pow(rightY, 2) + pow(rightZ, 2));
	newVecZ = rightZ / sqrt(pow(rightX, 2) + pow(rightY, 2) + pow(rightZ, 2));

	rightX = newVecX;
	rightY = newVecY;
	rightZ = newVecZ;


	//right X lookat

	upX = rightY*vecZ - rightZ*vecY;
	upY = rightZ*vecX - rightX*vecZ;
	upZ = rightX*vecY - rightY*vecX;

	newVecX = upX / sqrt(pow(upX, 2) + pow(upY, 2) + pow(upZ, 2));
	newVecY = upY / sqrt(pow(upX, 2) + pow(upY, 2) + pow(upZ, 2));
	newVecZ = upZ / sqrt(pow(upX, 2) + pow(upY, 2) + pow(upZ, 2));

	upX = newVecX;
	upY = newVecY;
	upZ = newVecZ;

}

void Mapper::buildString(){
	stringstream ss (stringstream::in | stringstream::out);

	ss << eyeX;

	s = ss.str();

	s.append(",");

	ss.str("");
	ss.clear();

	ss << eyeY;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << eyeZ;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << centerX;

	s.append(ss.str());
	s.append(",");

	ss.str("");
	ss.clear();

	ss << centerY;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << centerZ;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << upX;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << upY;

	s.append(ss.str());

	s.append(",");

	ss.str("");
	ss.clear();

	ss << upZ;

	s.append(ss.str());

	s.append(",");

	coordString = new char[s.length() + 1];
	strcpy(coordString, s.c_str());
}

void Mapper::clearGlobals(){
	cDatBig = NULL;
	cDatSmall = NULL;
	s.clear();
	coordString = NULL;
}