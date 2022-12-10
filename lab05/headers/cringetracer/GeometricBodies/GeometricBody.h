#pragma once

#include "../HeyraMath/HAffine.h"
#include "../Ray.h"

#include "../../pch.h"
#include "../../geometry/primitives3d/structers.h"
class GeometricBody
{
protected:
	HVec<double> _color; // values: 0..255
	HAffine<double> _tform;

public:
	GeometricBody();
	GeometricBody(const ImVec3& inColor);

	~GeometricBody();

	inline void SetTransform(const HAffine<double>& transformation) { _tform = transformation; }
	inline void SetTransform(const HVec<double>& translation, const HVec<double>& rotation, const HVec<double>& scale) {
		_tform = HAffine<double>(translation, rotation, scale);
	}
	inline void SetColor(const HVec<double>& inColor) { _color = inColor; }

	inline const HVec<double>& GetColor() { return _color; }
	inline const HAffine<double>& GetTransform() { return _tform; }

	// for drawing wireframes
	virtual void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) = 0;
	virtual bool TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) = 0;
};
