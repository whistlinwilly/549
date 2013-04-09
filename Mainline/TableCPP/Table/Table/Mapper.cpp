#include "Mapper.h"
#include "DefinedObjects.h"
#include <math.h>

#define _USE_MATH_DEFINES

Mapper::Mapper(){
}
float Mapper::xToInches(float pixels){
	return ((pixels/TABLE_X)*TABLE_WIDTH);
}

float Mapper::yToInches(float pixels){
	return ((pixels/TABLE_Y)*TABLE_WIDTH);
}

float Mapper::toRads(float degrees){
	return ((degrees/360.0)*M_PI);
}

float Mapper::toDegrees(float radians){
	return ((radians/M_PI)*360.0);
}