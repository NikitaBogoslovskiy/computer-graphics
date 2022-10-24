#include "geometry/primitives3d/mesh.h"
#include "geometry/primitives3d/structers.h"
//#include <iostream>

Mesh::Mesh() : init_points(), points(), polygons() {
	translate_mat.setIdentity();
	rotate_mat.setIdentity();
	scale_mat.setIdentity();
	reflect_mat.setIdentity();
}

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
				draw_list->AddCircleFilled(start + offset, 3.f, IM_COL32(255, 0, 0, 255), 10);
			}
		}
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
	float x = vec.x, y = vec.y, z = vec.z;
	angle = angle * PI / 180;
	float cosa = cos(-angle), sina = sin(-angle);
	Eigen::Matrix<float, 4, 4> rmat{
		{1 - cosa * x * x + cosa, 1 - cosa * x * y + sina * z, 1 - cosa * x * z - sina * y, 0},
		{1 - cosa * x * y - sina * z, 1 - cosa * y * y + cosa, 1 - cosa * y * z + sina * x, 0},
		{1 - cosa * x * z + sina * y, 1 - cosa * y * z - sina * x, 1 - cosa * z * z + cosa, 0},
		{0, 0, 0, 1}
	};
	rotate_mat = rotate_mat * rmat;
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
