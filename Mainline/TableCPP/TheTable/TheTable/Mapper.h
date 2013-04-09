#include "DefinedObjects.h"

using namespace std;

class Mapper
{
public:
	Mapper();

	float distanceFromTable;
	float incidentAngle;
	float twist;
	float rotation;
	ProDat proDat[NUM_PROJECTORS];
	circleData* cDatSmall;
	circleData* cDatBig;
	float eyeX, eyeY, eyeZ;
	float centerX, centerY, centerZ;
	float upX, upY, upZ;
	int isFirst;
	float rotationAdd;

	std::string s;
	char *coordString;

	void clearGlobals(void);
	float xToInches(float pixels);
	float yToInches(float pixels);
	float xToPixels(float inches);
	float yToPixels(float inches);
	float toRads(float degrees);
	float toDegrees(float radians);

	void resetGlobals(void);
	void computeProjectorData(void);
	void computeUpVector(void);
	void upFromTwist();
	void buildString();
};