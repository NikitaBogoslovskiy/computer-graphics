#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"
#include "geometry/camera.h"

void Mesh::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	//throw std::exception("Not implemented");
	for (auto& facet : polygons) {
		//facet.draw(draw_list, origin);
		auto eye = ImVec3(200.f, 200.f, 200.f);
		auto projectionMatr = Camera::perspective(30.f, 4.f / 3.f, 1.f, 1000.f);
		auto viewMatr0 = Camera::lookAtRH(eye, points[1]);
		for (size_t i = 1; i <= facet.size(); i++)
		{
			auto ind = i % facet.size();
			auto viewMatr1 = Camera::lookAtRH(eye, points[ind + 1]);
			Eigen::Matrix<float, 4, 1> v0{ points[ind].x, points[ind].y, points[ind].z,  1.f };
			Eigen::Matrix<float, 4, 1> v1{ points[ind + 1].x, points[ind + 1].y, points[ind + 1].z,  1.f };

			auto transformation = projectionMatr * viewMatr0;
			auto pupu = projectionMatr * viewMatr0 * v0;// thus we projected v0 onto 2d canvas
			auto pipi = projectionMatr * viewMatr1 * v1;
			draw_list->AddLine(ImVec2(pupu(0,0), pupu(0,1)) + offset, ImVec2(pipi(0, 0), pipi(0, 1)) + offset, IM_COL32(0, 255, 0, 255), 2.f /*thickness*/);
			draw_list->AddCircleFilled(ImVec2(pupu(0, 0), pupu(0, 1)) + offset, 4.f, IM_COL32(0, 255, 0, 255), 10);

			viewMatr0 = viewMatr1;
		}
	}
}
