#pragma once

#include "../Ray.h"

#include "../../pch.h"
#include "../../geometry/primitives3d/structers.h"
class GeometricBody
{
public:
	//ImVec3 color;
	GeometricBody();
	~GeometricBody();

	// for drawing wireframes
	virtual void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) = 0;
	virtual bool TestIntersection(const Ray& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) = 0;

	HVec<double> color;
};
