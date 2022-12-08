#pragma once

#include "../HeyraMath/HVec.h"
#include "GeometricBody.h"

class Sphere: public GeometricBody
{
	//ImU32 color;
	HVec<double> _centre;
	double _r;

public:
	const double& Radius();
	const HVec<double>& Centre();

	Sphere(const double x0, const double y0, const double z0, const double radius, const ImVec3& color);
	~Sphere();

	void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) override;
	bool TestIntersection(const Ray<double>& ray, HVec<double>& intersection, HVec<double>& localNormal, HVec<double>& localColor) override;
};
