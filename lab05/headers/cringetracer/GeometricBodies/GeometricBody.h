#pragma once
#include "../../pch.h"
#include "../../geometry/primitives3d/structers.h"
class GeometricBody
{
public:
	ImVec3 color;
	GeometricBody();
	~GeometricBody();
	virtual void Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) = 0;
private:

};
