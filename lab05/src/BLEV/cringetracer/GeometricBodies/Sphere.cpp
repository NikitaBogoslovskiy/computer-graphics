#include "../headers/cringetracer/GeometricBodies/Sphere.h"

inline const double& Sphere::Radius()
{
	return RadiusX();
}

Sphere::Sphere(const double x0, const double y0, const double z0, const double radius, const ImVec3& inColor)
	: Ellipsoid(x0, y0, z0, radius, radius, radius, inColor)
{}
