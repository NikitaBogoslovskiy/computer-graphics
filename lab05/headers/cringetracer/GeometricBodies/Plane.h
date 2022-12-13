#pragma once

#include "../HeyraMath/HVec.h"
#include "GeometricBody.h"

class Plane : public GeometricBody
{
	// unit plane [-1 1], always centered at the origin and lies in xy plane in its local coordinates
public:
	Plane(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor);
	~Plane();

	void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) override;
	bool TestIntersection(const Ray<double>& ray, HVec<double>& intersection, HVec<double>& localNormal, HVec<double>& localColor) const override;
};
