#pragma once
class WowVector3d
{
public:
	WowVector3d();
	WowVector3d(double _x, double _y, double _z);
	WowVector3d(double _x, double _y);
	~WowVector3d();

	double distance(const WowVector3d &vector);
	double distance2d(const WowVector3d &vector);
	void set(double _x, double _y, double _z);

	double x;
	double y;
	double z;
};

