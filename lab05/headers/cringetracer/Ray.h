#pragma once
#include "cringetracer/HeyraMath/HVec.h"

class Ray
{
	
public:
	HVec<double> p1;
	HVec<double> p2;
	HVec<double> direction;

	Ray();
	Ray(const HVec<double>& point1, const HVec<double>& point2);
	~Ray();

	//const HVec<double>& p1() const;
	//const HVec<double>& p2() const;
	//const HVec<double>& Direction() const;
};