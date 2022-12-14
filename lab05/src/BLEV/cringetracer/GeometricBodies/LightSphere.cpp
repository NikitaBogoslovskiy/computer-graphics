#include "cringetracer/GeometricBodies/LightSphere.h"


LightSphere::LightSphere(const HVec<double>& inPosition, double radius, const HVec<double>& inColor)
	: Sphere(inPosition.x, inPosition.y, inPosition.z, radius, ImVec3{ (float)inColor.x, (float)inColor.y, (float)inColor.z })
{
}

LightSphere::~LightSphere()
{
}

