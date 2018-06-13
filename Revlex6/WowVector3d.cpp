#include "stdafx.h"
#include "WowVector3d.h"
#include <math.h>

WowVector3d::WowVector3d()
	:x(0),
	y(0),
	z(0)
{
}


WowVector3d::WowVector3d(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}
WowVector3d::WowVector3d(double _x, double _y)
{
	x = _x;
	y = _y;
}

WowVector3d::~WowVector3d()
{
}

double WowVector3d::distance(const WowVector3d &vector)
{
	return  sqrt(pow((x - vector.x), 2) + pow((y - vector.y), 2) + pow((z - vector.z), 2));
}
double WowVector3d::distance2d(const WowVector3d &vector)
{
	return  sqrt(pow((x - vector.x), 2) + pow((y - vector.y), 2));
}
void WowVector3d::set(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}

WowVector3d operator+(const WowVector3d &vector1, const WowVector3d &vector2)
{
	WowVector3d vector3(0.0, 0.0, 0.0);
	vector3.x = vector1.x + vector2.x;
	vector3.y = vector1.y + vector2.y;
	vector3.z = vector1.z + vector2.z;
	return vector3;
}
WowVector3d operator-(const WowVector3d &vector1, const WowVector3d &vector2)
{
	WowVector3d vector3(0.0, 0.0, 0.0);
	vector3.x = vector1.x - vector2.x;
	vector3.y = vector1.y - vector2.y;
	vector3.z = vector1.z - vector2.z;
	return vector3;
}