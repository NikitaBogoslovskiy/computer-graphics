#pragma once
#include "GeometricBody.h"
class Cylinder : public GeometricBody
{
	HVec<double> _centre;
	double _rx, _h, _rz;
public:
	Cylinder(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor);
	~Cylinder();
	void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) override;
	bool TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) override;
private:

};
