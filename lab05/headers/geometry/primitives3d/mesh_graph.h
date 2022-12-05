#pragma once

#include "mesh.h"
#include "../methods/blev_math.h"

class MeshGraph : public Mesh
{
public:
	MeshGraph(const float& x0, const float& x1, const float& z0, const float& z1, const int& x_parts, const int& z_parts, Func3d ft) {
		fillFuncPoints(this, x0, x1, z0, z1, x_parts, z_parts, ft);
		use_normals = false;
	}
private:
	void fillFuncPoints(MeshGraph* prim, const float& x0, const float& x1, const float& z0, const float& z1, const int& x_parts, const int& z_parts, Func3d ft);
};
