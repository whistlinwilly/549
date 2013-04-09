class Mapper
{
public:
	Mapper();

	float distanceFromTable;
	float incidentAngle;
	float twist;
	float rotation;
	ProDat proDat[NUM_PROJECTORS];
	float eyeX, eyeY, eyeZ;
	float centerX, centerY;
	float upX, upY, upZ;

	float xToInches(float pixels);
	float yToInches(float pixels);
	float toRads(float degrees);
	float toDegrees(float radians);
	ProDat computeProjectorData(void);
};