#pragma once
#include "GeometricBody.h"
class Cone : public GeometricBody
{
	HVec<double> _centre;
	double _rx, _h, _rz;
public:
	Cone(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor);
	~Cone();
	void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) override;
	bool TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) const override;
private:

};