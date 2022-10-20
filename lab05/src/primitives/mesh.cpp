#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"

void Mesh::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	/* // DO NOT DELETE
	auto eye = ImVec3(200.f, 200.f, 200.f);
	auto projectionMatr = Camera::perspective(30.f, 4.f / 3.f, 1.f, 1000.f); // for EXAMPLE
	auto viewMatr = Camera::lookAtRH(eye, points[1]);
	auto vp = projectionMatr * viewMatr;
	for (auto& facet : polygons) {
		for (size_t i = 1; i <= facet.size(); i++)
		{
			auto ind0 = i % facet.size();
			auto ind1 = (ind0 + 1) % facet.size();
			Eigen::Matrix<float, 4, 1> v0{ points[ind0].x, points[ind0].y, points[ind0].z,  1.f }; // COLUMN-VEC
			Eigen::Matrix<float, 4, 1> v1{ points[ind1].x, points[ind1].y, points[ind1].z,  1.f };

			auto pupu = vp * v0;// thus we projected v0 onto 2d canvas
			auto pipi = vp * v1;
			draw_list->AddLine(ImVec2(pupu(0,0), pupu(1,0)) + offset, ImVec2(pipi(0, 0), pipi(0, 1)) + offset, IM_COL32(0, 255, 0, 255), 2.f);
			draw_list->AddCircleFilled(ImVec2(pupu(0, 0), pupu(1, 0)) + offset, 4.f, IM_COL32(0, 255, 0, 255), 10);
		}
	}*/
}
