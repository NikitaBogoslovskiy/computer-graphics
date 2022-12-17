#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"
#include <fstream>
#include <sstream>
#include <string>
#include "geometry/methods/funcs.h"
#include <thread>
#include <mutex>
#include <regex>

extern bool needRefresh;

Mesh::Mesh() : points(), polygons() {
	face_color = ImVec4(rand() % 256, rand() % 256, rand() % 256, 255);
	edge_color = ImVec4(255, 255, 204, 255);
	needRefresh = true;
}

std::mutex g_l_draw_list;

void Mesh::_draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir, size_t start, size_t end)
{
	std::vector<ImVec2> buf(10);
	for (size_t p1 = start; p1 < end; p1++) {
		auto& pol = polygons[p1];
		if (!use_normals || pol.normal * cam_dir < 0)
		{
			for (size_t i = 0; i < pol.size(); i++) {
				Eigen::Vector4f v0{ points[pol[i]].x, points[pol[i]].y, points[pol[i]].z,  1.f }; // COLUMN-VEC
				Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
				buf[i] = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3)) + offset;
			}

			//	#pragma omp critical
			{
				for (size_t i = 1; i < polygons[p1].size(); i++) {
					//const std::lock_guard<std::mutex> lock(g_l_draw_list);
					draw_list->AddLine(buf[i], buf[i - 1], color, thickness);
					//draw_list->AddCircleFilled(start + offset, 3.f, IM_COL32(255, 0, 0, 255), 10);
				}
				//const std::lock_guard<std::mutex> lock(g_l_draw_list);
				draw_list->AddLine(buf[0], buf[polygons[p1].size() - 1], color, thickness);
				//draw_list->AddConvexPolyFilled(buf.data(), polygon.size(), IM_COL32(155, 155, 155, 255));
			}
		}

	}
}

void Mesh::draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir)
{
	if (show) {
		_draw(draw_list, offset, vp, cam_dir, 0, polygons.size());
		//std::thread th1(&Mesh::_draw, this, draw_list, offset, vp, cam_dir, 0, polygons.size() / 4);
		//std::thread th2(&Mesh::_draw, this, draw_list, offset, vp, cam_dir, polygons.size() / 4, polygons.size() / 2);
		//std::thread th3(&Mesh::_draw, this, draw_list, offset, vp, cam_dir, polygons.size() / 2, polygons.size() / 4 * 3);
		//std::thread th4(&Mesh::_draw, this, draw_list, offset, vp, cam_dir, polygons.size() / 4 * 3, polygons.size());
		
		//th1.join();
		//th2.join();
		//th3.join();
		//th4.join();
		//th4.join();
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
	char slash;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
			
		std::string type;
		if (iss >> type, infile.eof()) {
			break;
		}

		if (type == "v") {
			ImVec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			m.add_point(std::move(vertex));
		}
		else if (type == "vt") {
			ImVec2 vertex;
			iss >> vertex.x >> vertex.y;
			if (vertex.x < 0) vertex.x += 1;
			if (vertex.y < 0) vertex.y += 1;
			if (vertex.x > 1) vertex.x -= 1;
			if (vertex.y > 1) vertex.y -= 1;
			vt.push_back(std::move(vertex));
		}
		else if (type == "f") {
			Polygon p;
			uint32_t vi, vti;
			std::string buf;
			while (iss >> buf) {
				std::stringstream iss2(buf);

				// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
				iss2 >> vi >> slash >> vti; // dont look for normals
				p.push_back(vi - 1);
				if(vti)	p.push_back_uv(vti - 1);
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
	this->polygons = std::move(m.polygons);
}

void Mesh::calculate_normal(Polygon& p)
{
	p.normal = normilize(cross_product(points[p.indices[2]] - points[p.indices[1]], points[p.indices[0]] - points[p.indices[1]]));
	p.d = -(p.normal * points[p.indices[0]]);
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
	Eigen::Matrix<float, 4, 4> mat{
		{1, 0, 0, 0},
		{0, cos(-angle), sin(-angle), 0},
		{0, -sin(-angle), cos(-angle), 0},
		{0, 0, 0, 1}
	};
	updatePoints(mat);
}

void Mesh::rotateY(float angle)
{
	angle = angle * PI / 180;
	Eigen::Matrix<float, 4, 4> mat{
		{cos(-angle), 0, -sin(-angle), 0},
		{0, 1, 0, 0},
		{sin(-angle), 0, cos(-angle), 0},
		{0, 0, 0, 1}
	};
	updatePoints(mat);
}

void Mesh::rotateZ(float angle)
{
	angle = angle * PI / 180;
	Eigen::Matrix<float, 4, 4> mat{
		{cos(-angle), sin(-angle), 0, 0},
		{-sin(-angle), cos(-angle), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	updatePoints(mat);
}

void Mesh::rotateU(ImVec3 p1, ImVec3 p2, float angle)
{
	auto vec = p2 - p1;
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec = vec / length;
	float x = vec.x, y = vec.y, z = vec.z;
	angle = angle * PI / 180;
	float cosa = cos(-angle), sina = sin(-angle);
	Eigen::Matrix<float, 4, 4> mat{
		{cosa + x * x * (1 - cosa),      x * y * (1 - cosa) - z * sina,  x * z * (1 - cosa) + y * sina, 0},
		{y * x * (1 - cosa) + z * sina,  cosa + y * y * (1 - cosa),      y * z * (1 - cosa) - x * sina, 0},
		{z * x * (1 - cosa) - y * sina,  z * y * (1 - cosa) + x * sina,  cosa + z * z * (1 - cosa),     0},
		{           0,                              0,                           0,                     1}
	};
	updatePoints(mat);
}

void Mesh::reflectX() {
	Eigen::Matrix<float, 4, 4> mat{
		{-1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	for (size_t i = 0; i < polygons.size(); ++i)
		std::reverse(polygons[i].indices.begin(), polygons[i].indices.end());
	updatePoints(mat, false);
}

void Mesh::reflectY() {
	Eigen::Matrix<float, 4, 4> mat{
		{1, 0, 0, 0},
		{0, -1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	for (size_t i = 0; i < polygons.size(); ++i)
		std::reverse(polygons[i].indices.begin(), polygons[i].indices.end());
	updatePoints(mat, false);
}

void Mesh::reflectZ() {
	Eigen::Matrix<float, 4, 4> mat{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, -1, 0},
		{0, 0, 0, 1}
	};
	for (size_t i = 0; i < polygons.size(); ++i)
		std::reverse(polygons[i].indices.begin(), polygons[i].indices.end());
	updatePoints(mat, false);
}

void Mesh::translate(float dx, float dy, float dz) {
	Eigen::Matrix<float, 4, 4> mat{
		{1, 0, 0, dx},
		{0, 1, 0, dy},
		{0, 0, 1, dz},
		{0, 0, 0, 1}
	};
	updatePoints(mat, false);
}

void Mesh::scale(float dx, float dy, float dz)
{
	Eigen::Matrix<float, 4, 4> mat{
		{dx, 0, 0, 0},
		{0, dy, 0, 0},
		{0, 0, dz, 0},
		{0, 0, 0,  1}
	};
	updatePoints(mat);
}

void Mesh::updatePoints(Eigen::Matrix<float, 4, 4>& mat, bool needTranslsate)
{
	Eigen::MatrixXf points_matrix;
	points_matrix.resize(4, points.size());
	for (size_t i = 0; i < points.size(); i++) {
		points_matrix(0, i) = points[i].x;
		points_matrix(1, i) = points[i].y;
		points_matrix(2, i) = points[i].z;
		points_matrix(3, i) = 1;
	}

	Eigen::MatrixXf result_matrix;
	if (!needTranslsate)
		result_matrix = mat * points_matrix;
	else
	{
		auto c = center();
		Eigen::Matrix<float, 4, 4> to_center{
			{1, 0, 0, -c.x},
			{0, 1, 0, -c.y},
			{0, 0, 1, -c.z},
			{0, 0, 0, 1}
		};
		Eigen::Matrix<float, 4, 4> from_center{
			{1, 0, 0, c.x},
			{0, 1, 0, c.y},
			{0, 0, 1, c.z},
			{0, 0, 0, 1}
		};
		result_matrix = from_center * mat * to_center * points_matrix;
	}

	for (size_t i = 0; i < points.size(); i++) {
		points[i].x = result_matrix(0, i);
		points[i].y = result_matrix(1, i);
		points[i].z = result_matrix(2, i);
	}
	needRefresh = true;
	recalculate_normals();
}

void Mesh::reverse()
{
	for (auto& p : polygons) {
		std::reverse(p.indices.begin(), p.indices.end());
		std::reverse(p.uv_ind.begin(), p.uv_ind.end());
	}
	recalculate_normals();
}


void Mesh::save_s(const char* filename, const std::set<Mesh*>& meshes) {
	std::ofstream out(filename);
	if (!out.is_open())
	{
		printf("something went wrong\n");
	}

	size_t points = 0;
	size_t polygons = 0;
	for (Mesh* m : meshes)
	{

		for (auto& p : m->points) {
			out << "v " << p.x << ' ' << p.y << ' ' << p.z << '\n';
		}
		out << '\n';

		out << "usemtl " << m << "\n";

		for (auto& p : m->polygons) {
			out << 'f';
			for (auto i : p.indices) {
				out << ' ' << i + 1 + points;
			}
			out << '\n';
		}
	
		points += m->points.size();
		polygons += m->polygons.size();
	}
	out << "# Vertices: " << points << "\n# Faces: " << polygons << "\n\n";
	
	out.close();
}

void Mesh::open_s(const char* filename, std::vector<Mesh*>& meshes)
{
	std::ifstream infile(filename);
	if (!infile.is_open()) {
		printf("path is wrong\n");
		return;
	}

	std::vector<ImVec3> points;
	std::vector<ImVec2> vt;

	Mesh* m = NULL;
	std::string line;
	char slash;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		std::string type;
		if (iss >> type, infile.eof()) {
			break;
		}

		if (type == "v") {
			ImVec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			points.push_back(std::move(vertex));
		}
		else if (type == "vt") {
			ImVec2 vertex;
			iss >> vertex.x >> vertex.y;
			if (vertex.x < 0) vertex.x += 1;
			if (vertex.y < 0) vertex.y += 1;
			if (vertex.x > 1) vertex.x -= 1;
			if (vertex.y > 1) vertex.y -= 1;
			vt.push_back(std::move(vertex));
		}
		else if (type == "f") {
			if (!m) {
				m = new Mesh();
				m->points = points;
				m->vt = vt;
			}
			Polygon p;
			uint32_t vi, vti;
			std::string buf;
			while (iss >> buf) {
				std::stringstream iss2(buf);

				// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
				iss2 >> vi >> slash >> vti; // dont look for normals
				p.push_back(vi - 1);
				if (vti) p.push_back_uv(vti - 1);
			}
			if (p.size() < 3) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			m->add_polygon(std::move(p));
		}
		else if (type == "usemtl") {
			if (m) {
				m->recalculate_normals();
				m->face_color = { (float)(rand() % 255), (float)(rand() % 255), (float)(rand() % 255), 255.f };
				meshes.push_back(m);
			}
			m = new Mesh();
			m->points = points;
			m->vt = vt;
		}
	}
	infile.close();

	if (m) {
		m->recalculate_normals();
		m->face_color = { (float)(rand() % 255), (float)(rand() % 255), (float)(rand() % 255), 255.f };
		meshes.push_back(m);
	}
}

std::pair<float, ImVec3> Mesh::is_intersected_with_ray(const ImVec3& sp, const ImVec3& direction, bool use_normals) const
{
	std::pair<float, ImVec3> ret = { FLT_MAX, 0 };
	float t;
	for (const Polygon& p : polygons)
	{
		if (use_normals && dot_product(direction, p.normal) >= 0) continue;
		if (BarTest(p, sp, direction, t)) {
			if (t < ret.first) {
				ret.first = t;
				ret.second = p.normal;
			}
		}
	}
	return ret;
}

bool Mesh::is_intersected_with_light(const ImVec3& sp, const ImVec3& direction, bool use_normals, float max_t) const
{
	float t;
	for (const Polygon& p : polygons)
	{
		if (use_normals && dot_product(direction, p.normal) >= 0) continue;
		if (BarTest(p, sp, direction, t)) {
			if (t < max_t) {
				return true;
			}
		}
	}
	return false;
}

bool Mesh::BarTest(const Polygon& p, const ImVec3& sp, const ImVec3& direction, float &ret) const
{
	auto v0 = points[p.indices[0]];
	for (size_t i = 2; i < p.size(); i++)
	{
		const float EPSILON = 0.0000001;
		auto E1 = points[p.indices[i-1]] - v0;
		auto E2 = points[p.indices[i]] - v0;
		auto P = cross_product(direction, E2);
		auto temp = (P * E1);
		if (temp > -EPSILON && temp < EPSILON) continue;
	
		auto T = sp - v0;
		auto u = dot_product(P,T) / temp;
		if (u < 0.f || u > 1.f) continue;

		auto Q = cross_product(T, E1);
		auto v = dot_product(Q, direction) / temp;
		if (v < 0.f || u + v > 1.f) continue;

		auto t = dot_product(Q, E2) / temp;
		if (t > EPSILON)
		{
			ret = t;
			return true;
		}
		else continue;
	}
	return false;
}


