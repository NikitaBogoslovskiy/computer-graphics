#pragma once
#include <memory>

//#include "../Materials/Material.h"
#include "../HeyraMath/HAffine.h"
#include "../Ray.h"

#include "../../pch.h"
#include "../../geometry/primitives3d/structers.h"

class Material; // OOF no loops

class GeometricBody
{

protected:

	HVec<double> _color; // values: 0..255
	HAffine<double> _tform;

public:
	HVec<double> Origin{ 0.0,0.0,0.0 };
	HVec<double> Rotation{ 0.0,0.0,0.0 };
	HVec<double> Scale{ 1.0,1.0,1.0 };

	Material* Mtl = nullptr;

	GeometricBody(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor);
	~GeometricBody();

	inline bool HasMaterial() { return Mtl != nullptr; }
	inline void SetMaterial(const Material* mtl) { Mtl = const_cast<Material*>(mtl); }

	inline void SetTransform(const HAffine<double>& transformation) { _tform = transformation; }
	inline void SetTransform() {
		//_tform = HAffine<double>(translation, rotation, scale);
		_tform = HAffine<double>(Origin, Rotation, Scale);
	}
	inline void SetColor(const HVec<double>& inColor) { _color = inColor; }

	inline const HVec<double>& GetColor() { return _color; }
	inline const HAffine<double>& GetTransform() { return _tform; }

	// for drawing wireframes
	virtual void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) = 0;
	virtual bool TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) const = 0;
};
