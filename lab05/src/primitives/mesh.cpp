#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"

void Mesh::draw(ImDrawList* draw_list, const ImVec2& offset, Eigen::Matrix4f& vp)
{
	for (auto& facet : polygons) {
		for (size_t i = 1; i <= facet.size(); i++)
		{
			auto ind0 = i % facet.size();
			auto ind1 = (ind0 + 1) % facet.size();
			Eigen::Vector4f v0{ points[ind0].x, points[ind0].y, points[ind0].z,  1.f }; // COLUMN-VEC
			Eigen::Vector4f v1{ points[ind1].x, points[ind1].y, points[ind1].z,  1.f };

			Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
			Eigen::Vector4f v1_2d = vp * v1;

			auto start = ImVec2(v0_2d(0), v0_2d(1));
			auto end = ImVec2(v1_2d(0), v1_2d(1));
			draw_list->AddLine(start + offset, end + offset, IM_COL32(0, 255, 0, 255), 1.f);
			draw_list->AddCircleFilled(start + offset, 3.f, IM_COL32(0, 255, 0, 255), 10);
		}
	}
}
