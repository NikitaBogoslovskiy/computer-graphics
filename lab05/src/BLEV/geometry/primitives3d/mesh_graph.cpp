#include "../headers/geometry/primitives3d/mesh_graph.h"
#include "../headers/geometry/methods/blev_math.h"

void MeshGraph::fillFuncPoints(MeshGraph* prim, const float& x0, const float& x1, const float& z0, const float& z1, const int& x_parts, const int& z_parts, Func3d fn)
{
	float dx = abs(x0 - x1) / x_parts,
		dz = abs(z0 - z1) / z_parts;

	for (unsigned int i = 0; i <= x_parts; i++) {
		auto x = x0 + dx * i;
		for (unsigned int j = 0; j <= z_parts; j++) {
			auto z = z0 + dz * j;
			add_point(ImVec3(x, fn(x, z), z));
		}
	}

	int side = z_parts + 1;
	for (unsigned int i = 0; i <= x_parts - 1; i++) {
		for (unsigned int j = 0; j <= z_parts - 1; j++) {
			add_polygon(Polygon{ i * side + j,  i * side + (j + 1),   (i + 1) * side + (j + 1),   (i + 1) * side + j });
		}
	}
}
