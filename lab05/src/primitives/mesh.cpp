#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"
#include <fstream>
#include <sstream>
#include <string>

//#include <iostream>
void Mesh::draw(ImDrawList* draw_list, const ImVec2& offset, Eigen::Matrix4f& vp)
{
	if (show) {
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

				auto start = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3));
				auto end = ImVec2(v1_2d(0) / v1_2d(3), v1_2d(1) / v1_2d(3));

				//start.x *= 512.f * 0.5f; start.y *= 512.f * 0.5f; //if we use fov perspective
				//end.x *= 512.f * 0.5f; end.y *= 512.f * 0.5f;

				draw_list->AddLine(start + offset, end + offset, color, thickness);
				//draw_list->AddCircleFilled(start + offset, 3.f, IM_COL32(255, 0, 0, 255), 10);
			}
		}
	}
}

void Mesh::save(const char* filename)
{
}

void Mesh::open(const char* filename)
{
	Mesh m;
	std::ifstream infile(filename);
	if (!infile.is_open()) {
		printf("path is wrong\n");
		return;
	}

	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
			
		std::string type;
		if (iss >> type, infile.eof()) {
			this->points = std::move(m.points);
			this->polygons = std::move(m.polygons);
			return;
		}

		if (type == "v") {
			ImVec3 vertex;
			if (!(iss >> vertex.x >> vertex.y >> vertex.z)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			m.add_point(std::move(vertex * 5.f));
		}
		else if (type == "f") {
			Polygon p;
			uint32_t temp;
			std::string buf;
			while (iss >> buf) {
				std::istringstream iss2(buf);
				if (!(iss2 >> temp)) {
					printf("File can't be read by our simple parser\n");
					return;
				}
				p.push_back(temp - 1);
			}
			if (p.size() < 3) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			m.add_polygon(std::move(p));
		}
	}
	this->points = std::move(m.points);
	this->polygons = std::move(m.polygons);
}
