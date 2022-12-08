#include "cringetracer/GeometricBodies/GeometricBody.h"

GeometricBody::GeometricBody()
{
}

GeometricBody::~GeometricBody()
{
}

void GeometricBody::SetTransform(const HAffine<double>& transformation)
{
	_tform = transformation;
}
