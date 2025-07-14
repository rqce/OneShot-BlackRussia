#include "math.h"

float GetAngleBetweenTwoPoints(float x1, float y1, float x2, float y2)
{
	float plus = 0.f;
	int mode = 1;
	
	if((x1 < x2) && (y1 > y2))
	{
		plus = M_PI/2.f;
		mode = 2;
	}
	if((x1 < x2) && (y1 < y2))
	{
		plus = M_PI;
	}
	if((x1 > x2) && (y1 < y2))
	{
		plus = M_PI*1.5f;
		mode = 2;
	}
	
	float lx = x2 - x1;
	float ly = y2 - y1;
	lx = abs(lx);
	ly = abs(ly);
	if(mode == 1)
	{
		ly = ly/lx;
	} else {
		ly = lx/ly;
	}
	
	ly = atan(ly);
	ly = ly + plus;
	
	return ly;
}

float GetRadianAngleBetweenPoints(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	
	float ang = (((atan2(y, x) * (180.f/M_PI))) * M_PI) / 180.f;
		
	return ang;
}

float Get3dDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
  return sqrt(pow(x2-x1,2.0f) +
	            pow(y2-y1,2.0f) +
				pow(z2-z1,2.0f));
}

float Get2dDistance(Vec3 vec1, Vec3 vec2)
{
	return sqrt(pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec2.y, 2) * 1.0);
}
