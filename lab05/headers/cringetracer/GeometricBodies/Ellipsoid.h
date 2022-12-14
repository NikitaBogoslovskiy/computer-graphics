#pragma once
#include "../HeyraMath/HVec.h"
#include "GeometricBody.h"

class Ellipsoid : public GeometricBody
{
public:
	inline const double& RadiusX();
	inline const double& RadiusY();
	inline const double& RadiusZ();

	//inline const HVec<double>& Centre();

	Ellipsoid(const double x0, const double y0, const double z0, 
			  const double rX, const double rY, const double rZ, 
			  const ImVec3& inColor);

	~Ellipsoid();

	void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) override;
	bool TestIntersection(const Ray<double>& ray, HVec<double>& intersection, HVec<double>& localNormal, HVec<double>& localColor) const override;
};