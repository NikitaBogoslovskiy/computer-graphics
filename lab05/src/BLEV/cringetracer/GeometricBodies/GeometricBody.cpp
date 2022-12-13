#include "cringetracer/GeometricBodies/GeometricBody.h"

GeometricBody::GeometricBody(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor) {
	_color = HVec<double>{ (double)inColor.x, (double)inColor.y, (double)inColor.z };
	Origin = origin;
	Rotation = rotation;
	Scale = scale;
	this->SetTransform();
}

GeometricBody::~GeometricBody() {}
