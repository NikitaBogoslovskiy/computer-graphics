#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"
#include <fstream>
#include <sstream>
#include <string>
#include "geometry/methods/funcs.h"
//#include <omp.h>
#include <thread>
#include <mutex>

Mesh::Mesh() : init_points(), points(), polygons() {
	translate_mat.setIdentity();
	rotate_mat.setIdentity();
	scale_mat.setIdentity();
	reflect_mat.setIdentity();
}

std::mutex g_l_draw_list;

void Mesh::_draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir, size_t start, size_t end)
{
	std::vector<ImVec2> buf(10);
	for (size_t p1 = start; p1 < end; p1++) {
		auto& pol = polygons[p1];
		if (pol.normal * cam_dir < 0) {
			for (size_t i = 0; i < pol.size(); i++) {
				Eigen::Vector4f v0{ points[pol[i]].x, points[pol[i]].y, points[pol[i]].z,  1.f }; // COLUMN-VEC
				Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
				buf[i] = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3)) + offset;
			}

			//	#pragma omp critical
			{
				for (size_t i = 1; i < polygons[p1].size(); i++) {
					const std::lock_guard<std::mutex> lock(g_l_draw_list);
					draw_list->AddLine(buf[i], buf[i - 1], color, thickness);
					//draw_list->AddCircleFilled(start + offset, 3.f, IM_COL32(255, 0, 0, 255), 10);
				}
				const std::lock_guard<std::mutex> lock(g_l_draw_list);
				draw_list->AddLine(buf[0], buf[polygons[p1].size() - 1], color, thickness);
				//draw_list->AddConvexPolyFilled(buf.data(), polygon.size(), IM_COL32(155, 155, 155, 255));
			}
		}

	}
}

void Mesh::draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir)
{
	if (show) {
		std::thread th1(&Mesh::_draw, this, draw_list, offset, vp, cam_dir, 0, polygons.size() / 3);
		std::thread th2(&Mesh::_draw, this, draw_list, offset, vp, cam_dir, polygons.size() / 3, polygons.size() / 3 * 2);
		std::thread th4(&Mesh::_draw, this, draw_list, offset, vp, cam_dir, polygons.size() / 3 * 2, polygons.size());

		th1.join();
		th2.join();
		th4.join();
		/*
			// draw normals
				auto c3 = polygon.center(points);
				Eigen::Vector4f v0{ c3.x, c3.y, c3.z, 1.f }; // COLUMN-VEC
				Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
				auto c2 = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3)) + offset;
			
				Eigen::Vector4f v0_2{ c3.x + polygon.normal.x, c3.y + polygon.normal.y, c3.z + polygon.normal.z,  1.f }; // COLUMN-VEC
				Eigen::Vector4f v0_2d_2 = vp * v0_2;// thus we projected v0 onto 2d canvas
				auto t = ImVec2(v0_2d_2(0) / v0_2d_2(3), v0_2d_2(1) / v0_2d_2(3)) + offset;
				draw_list->AddLine(c2, t, IM_COL32(255, 0, 0, 255), 1.f);
			*/
	}
}

void Mesh::save(const char* filename)
{
	std::ofstream out(filename);
	if (!out.is_open())
	{
		printf("something went wrong\n");
	}

	out << "# Vertices: " << points.size() << "\n# Faces: " << polygons.size() << "\n\n";

	for (auto& p : points) {
		out << "v " << p.x << ' ' << p.y << ' ' << p.z << '\n';
	}

	out << '\n';

	for (auto& p : polygons) {
		out << 'f';
		for (auto i : p.indices) {
			out << ' ' << i + 1;
		}
		out << '\n';
	}
	out.close();
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
			break;
		}

		if (type == "v") {
			ImVec3 vertex;
			if (!(iss >> vertex.x >> vertex.y >> vertex.z)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			m.add_point(std::move(vertex));
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
				/*
					if (p.size() < 3) {
				}
				else {
					m.add_polygon(p);
					std::swap(p[1], p[2]);
					p[2] = temp;
				}
				*/
				//char slash;
				//if (!(iss2 >> slash) || slash != '/') continue;
			}
			if (p.size() < 3) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			m.add_polygon(std::move(p));
		}
	}
	infile.close();

	this->points = std::move(m.points);
	this->init_points = std::move(m.init_points);
	this->polygons = std::move(m.polygons);
}

void Mesh::recalculate_normals()
{
	for (auto& p : polygons) {
		calculate_normal(p);
	}
}

void Mesh::rotateX(float angle)
{
	angle = angle * PI / 180;
	Eigen::Matrix<float, 4, 4> rmat{
		{1, 0, 0, 0},
		{0, cos(-angle), sin(-angle), 0},
		{0, -sin(-angle), cos(-angle), 0},
		{0, 0, 0, 1}
	};
	rotate_mat = rmat * rotate_mat;
	updatePoints();
}

void Mesh::rotateY(float angle)
{
	angle = angle * PI / 180;
	Eigen::Matrix<float, 4, 4> rmat{
		{cos(-angle), 0, -sin(-angle), 0},
		{0, 1, 0, 0},
		{sin(-angle), 0, cos(-angle), 0},
		{0, 0, 0, 1}
	};
	rotate_mat = rmat * rotate_mat;
	updatePoints();
}

void Mesh::rotateZ(float angle)
{
	angle = angle * PI / 180;
	Eigen::Matrix<float, 4, 4> rmat{
		{cos(-angle), sin(-angle), 0, 0},
		{-sin(-angle), cos(-angle), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	rotate_mat = rmat * rotate_mat;
	updatePoints();
}

void Mesh::rotateU(ImVec3 p1, ImVec3 p2, float angle)
{
	auto vec = p2 - p1;
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec = vec / length;
	float x = vec.x, y = vec.y, z = vec.z;
	angle = angle * PI / 180;
	float cosa = cos(-angle), sina = sin(-angle);
	Eigen::Matrix<float, 4, 4> rmat{
		{cosa + x * x * (1 - cosa),      x * y * (1 - cosa) - z * sina,  x * z * (1 - cosa) + y * sina, 0},
		{y * x * (1 - cosa) + z * sina,  cosa + y * y * (1 - cosa),      y * z * (1 - cosa) - x * sina, 0},
		{z * x * (1 - cosa) - y * sina,  z * y * (1 - cosa) + x * sina,  cosa + z * z * (1 - cosa),     0},
		{           0,                              0,                           0,                     1}
	};
	rotate_mat = rmat * rotate_mat;
	updatePoints();
}

void Mesh::scale(float dx, float dy, float dz)
{
	scale_mat(0, 0) *= dx;
	scale_mat(1, 1) *= dy;
	scale_mat(2, 2) *= dz;
	updatePoints();
}

void Mesh::updatePoints()
{
	Eigen::MatrixXf points_matrix;
	points_matrix.resize(4, init_points.size());
	for (size_t i = 0; i < init_points.size(); i++) {
		points_matrix(0, i) = init_points[i].x;
		points_matrix(1, i) = init_points[i].y;
		points_matrix(2, i) = init_points[i].z;
		points_matrix(3, i) = 1;
	}

	auto result_matrix = reflect_mat * translate_mat * rotate_mat * scale_mat * points_matrix;
	auto num = result_matrix.cols();

	for (size_t i = 0; i < points.size(); i++) {
		points[i].x = result_matrix(0, i);
		points[i].y = result_matrix(1, i);
		points[i].z = result_matrix(2, i);
	}
}

void Mesh::updateInitPoints()
{
	auto c = center();
	for (size_t i = 0; i < init_points.size(); i++) {
		init_points[i].x -= c.x;
		init_points[i].y -= c.y;
		init_points[i].z -= c.z;
	}
	translate_mat(0, 3) = c.x;
	translate_mat(1, 3) = c.y;
	translate_mat(2, 3) = c.z;
}
