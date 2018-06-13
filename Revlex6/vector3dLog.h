#pragma once
#include "WowVector3d.h"
class Vector3dLog
{
public:
	Vector3dLog();
	~Vector3dLog();

	int logLength = 10; // wieviel einträge das location-Log fasst
	WowVector3d sLoc;
	WowVector3d aLoc;
	WowVector3d lastLoc; // letzte location beim Durchlauf durch das objekt <map>lastVector3d
	WowVector3d nextLoc; // der wahrscheinlich nächste berechnte Punkt vom target
	WowVector3d locPerMsec; // die meter pro millisec vom target
	//WowObjectLight forCastTarget;
	unsigned long targetLastVectorCheck;
	unsigned long sTime = 0.0;
	unsigned long aTime = 0.0;
	unsigned long forCastTime = 0.0;
};

