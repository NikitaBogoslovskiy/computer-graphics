#include "../headers/geometry/primitives3d/mesh_graph.h"

static float ripples(const float& x, const float& z) {
	float r = x * x + z * z + 1;
	return 5 * (cos(r) / r + 0.1);
}

static float ripples2(const float& x, const float& z) {
	float r = x * x + z * z;
	return cos(r) / (r + 1);
}

static float sinxMultCosz(const float& x, const float& z) {
	return sin(x) * cos(z);
}

static float sinxPlusCosz(const float& x, const float& z) {
	return sin(x) + cos(z);
}

static float squaresSum(const float& x, const float& z) {
	return x * x + z * z;
}

typedef float (*Func3d)(const float&, const float&);

std::map<MeshGraph::FuncType, Func3d> funcsMap{
	{ MeshGraph::RIPPLES ,&ripples },
	{ MeshGraph::RIPPLES2 ,&ripples2 },
	{ MeshGraph::SINX_MULT_COSZ , &sinxMultCosz },
	{ MeshGraph::SINX_PLUS_COSZ , &sinxPlusCosz },
	{ MeshGraph::SQUARES_SUM ,&squaresSum },
};

void MeshGraph::fillFuncPoints(MeshGraph* prim, const float& x0, const float& x1, const float& z0, const float& z1, const int& x_parts, const int& z_parts, FuncType ft)
{
	float dx = abs(x0 - x1) / x_parts,
		dz = abs(z0 - z1) / z_parts;
	Func3d fn = funcsMap[ft];

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
