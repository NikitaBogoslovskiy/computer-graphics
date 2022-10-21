#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"
//#include <iostream>
void Mesh::draw(ImDrawList* draw_list, const ImVec2& offset, Eigen::Matrix4f& vp)
{
	for (auto& polygon : polygons) {
		for (size_t i = 1; i <= polygon.size(); i++)
		{
			auto ind0 = i % polygon.size();
			auto ind1 = (ind0 + 1) % polygon.size();
			auto point_ind0 = polygon[ind0];
			auto point_ind1 = polygon[ind1];

			Eigen::Vector4f v0{ points[point_ind0].x, points[point_ind0].y, points[point_ind0].z,  1.f }; // COLUMN-VEC
			Eigen::Vector4f v1{ points[point_ind1].x, points[point_ind1].y, points[point_ind1].z,  1.f };

			Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
			Eigen::Vector4f v1_2d = vp * v1;

			//std::cout << v0_2d(0) / v0_2d(3) << " " << v0_2d(1) / v0_2d(3) << " " << v0_2d(2) / v0_2d(3) << " " << v0_2d(3) / v0_2d(3) << std::endl;
			//std::cout << v1_2d(0) / v1_2d(3) << " " << v1_2d(1) / v1_2d(3) << " " << v1_2d(2) / v1_2d(3) << " " << v1_2d(3) / v1_2d(3) << std::endl;
			
			auto start = ImVec2(v0_2d(0), v0_2d(1));
			auto end = ImVec2(v1_2d(0) , v1_2d(1));
			draw_list->AddLine(start + offset, end + offset, IM_COL32(0, 255, 0, 255), 1.f);
			draw_list->AddCircleFilled(start + offset, 3.f, IM_COL32(255, 0, 0, 255), 10);
		}
	}
}
