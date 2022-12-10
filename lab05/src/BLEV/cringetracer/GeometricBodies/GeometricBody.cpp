#include "cringetracer/GeometricBodies/GeometricBody.h"

GeometricBody::GeometricBody() {}

GeometricBody::GeometricBody(const ImVec3& inColor) {
	_color = HVec<double>{ (double)inColor.x, (double)inColor.y, (double)inColor.z };
}

GeometricBody::~GeometricBody() {}
