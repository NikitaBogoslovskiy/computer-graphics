#include "geometry/primitives3d/mesh.h"

void Mesh::draw(ImDrawList* draw_list)
{
	throw std::exception("Not implemented");
	for (auto& facet : polygons) {
		facet.draw(draw_list);
	}
}
