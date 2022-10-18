#include "geometry/primitives3d/mesh.h"

void Mesh::draw(ImDrawList* draw_list)
{
	for (auto& facet : polygons) {
		facet.draw(draw_list);
	}
}
