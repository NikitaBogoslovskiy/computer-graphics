#pragma once
#include "GeometricBody.h"

// we already have cube class and no wish messing with 'em
class Box : public GeometricBody
{
	inline static std::array<HVec<double>, 6> normals{
		HVec<double>{ 0.0, 0.0, 1.0 },
		HVec<double>{ 0.0, 0.0, -1.0 },
		HVec<double>{ -1.0, 0.0, 0.0 },
		HVec<double>{ 1.0, 0.0, 0.0 },
		HVec<double>{ 0.0, -1.0, 0.0 },
		HVec<double>{ 0.0, 1.0, 0.0 },
	};

public:
	Box(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor);
	~Box();
	void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) override;
	bool TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) const override;


};