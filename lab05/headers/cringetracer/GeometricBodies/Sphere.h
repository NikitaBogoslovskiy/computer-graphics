#pragma once

#include "../HeyraMath/HVec.h"
#include "Ellipsoid.h"

class Sphere: public Ellipsoid
{
public:
	inline const double& Radius();
	Sphere(const double x0, const double y0, const double z0, const double radius, const ImVec3& color);
};
