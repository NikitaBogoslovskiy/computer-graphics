#include "../headers/cringetracer/GeometricBodies/Sphere.h"

Sphere::Sphere(const double x0, const double y0, const double z0, const double radius, const ImVec3& col)
{
	centre = HVec<double>{ x0, y0, z0 };
	r = radius;
	//color = IM_COL32(col.x * 255, col.y * 255, col.z * 255, 255);
	color = col;
}

Sphere::~Sphere()
{
}

const double& Sphere::Radius() {
	return r;
}

const HVec<double>& Sphere::Centre() {
	return centre;
}

void Sphere::Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) {
	float rFlt = static_cast<float> (r);
	float xFlt = static_cast<float> (centre.At(0));
	float yFlt = static_cast<float> (centre.At(1));
	float zFlt = static_cast<float> (centre.At(2));
	Eigen::Vector4f bottom3d = vp * Eigen::Vector4f{ static_cast<float> (centre.At(0) + r),  static_cast<float> (centre.At(1)),  static_cast<float> (centre.At(2)),  1.f };
	ImVec2 bottom2d = ImVec2(bottom3d(0) / bottom3d(3), bottom3d(1) / bottom3d(3)) + offset;

	ImVec2 prev_point = bottom2d;
	for (float phi = 0; phi <= 360; phi += 10.f) {
		auto lol1 = DegreesToRadians(phi);
		Eigen::Vector4f next3d = vp * Eigen::Vector4f{ xFlt + rFlt * cos(lol1), yFlt + rFlt * sin(lol1), zFlt,  1.f };
		ImVec2 next2d = ImVec2(next3d(0) / next3d(3), next3d(1) / next3d(3)) + offset;
		dl->AddLine(prev_point, next2d, IM_COL32(color.x * 255, color.y * 255, color.z * 255, 255));
		prev_point = next2d;
	}

	prev_point = bottom2d;
	for (float phi = 0; phi <= 360; phi += 10.f) {
		auto lol1 = DegreesToRadians(phi);
		Eigen::Vector4f next3d = vp * Eigen::Vector4f{ xFlt + rFlt * cos(lol1), yFlt, zFlt + rFlt * sin(lol1),  1.f };
		ImVec2 next2d = ImVec2(next3d(0) / next3d(3), next3d(1) / next3d(3)) + offset;
		dl->AddLine(prev_point, next2d, IM_COL32(color.x * 255, color.y * 255, color.z * 255, 255));
		prev_point = next2d;
	}
}