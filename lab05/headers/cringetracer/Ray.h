#pragma once
#include "cringetracer/HeyraMath/HVec.h"

class Ray
{
	HVec<double> _p1;
	HVec<double> _p2;
	HVec<double> _direction;
public:
	Ray(const HVec<double>& point1, const HVec<double>& point2);
	~Ray();

	const HVec<double>& p1() const;
	const HVec<double>& p2() const;
	const HVec<double>& Direction() const;
};