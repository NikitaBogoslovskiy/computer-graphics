#pragma once

#include "../HeyraMath/HAffine.h"
#include "../Ray.h"

#include "../../pch.h"
#include "../../geometry/primitives3d/structers.h"
class GeometricBody
{
public:
	//ImVec3 color;
	GeometricBody();
	~GeometricBody();

	void SetTransform(const HAffine<double>& transformation);

	// for drawing wireframes
	virtual void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) = 0;
	virtual bool TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) = 0;

	HVec<double> _color;
	HAffine<double> _tform;
};
