#pragma once

#include "mesh.h"

typedef float (*Func3d)(const float&, const float&);

class MeshGraph : public Mesh
{
public:
	enum FuncType {
		SINX_MULT_COSZ,
		SINX_PLUS_COSZ,
		SQUARES_SUM,
		RIPPLES,
		RIPPLES2
	};
	static FuncType funcTypes;

	MeshGraph(const float& x0, const float& x1, const float& z0, const float& z1, const int& x_parts, const int& z_parts, FuncType ft) {
		fillFuncPoints(this, x0, x1, z0, z1, x_parts, z_parts, ft);
		std::cout << "hello from graph mesh\n";
	}
private:
	void fillFuncPoints(MeshGraph* prim, const float& x0, const float& x1, const float& z0, const float& z1, const int& x_parts, const int& z_parts, FuncType ft);
};
