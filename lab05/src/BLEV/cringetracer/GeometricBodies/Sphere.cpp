#include "../headers/cringetracer/GeometricBodies/Sphere.h"

Sphere::Sphere(const double x0, const double y0, const double z0, const double inRadius, const ImVec3& inColor)
{
	_centre = HVec<double>{ x0, y0, z0 };
	_r = inRadius;
	_color = HVec<double>{ inColor.x * 255, inColor.y * 255, inColor.z * 255 };
	// no point rotating a sphere
	// should move 3deLlIpSeS with multiple radiuses and spheres to another class
	this->SetTransform(HAffine<double>(_centre, HVec<double>{ 0.0, 0.0, 0.0 }, HVec<double>{ _r, _r, _r }));
}

Sphere::~Sphere()
{
}

const double& Sphere::Radius() {
	return _r;
}

const HVec<double>& Sphere::Centre() {
	return _centre;
}

void Sphere::Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) {
	float rFlt = static_cast<float> (_r);
	float xFlt = static_cast<float> (_centre.At(0));
	float yFlt = static_cast<float> (_centre.At(1));
	float zFlt = static_cast<float> (_centre.At(2));
	Eigen::Vector4f bottom3d = vp * Eigen::Vector4f{ static_cast<float> (_centre.At(0) + _r),  static_cast<float> (_centre.At(1)),  static_cast<float> (_centre.At(2)),  1.f };
	ImVec2 bottom2d = ImVec2(bottom3d(0) / bottom3d(3), bottom3d(1) / bottom3d(3)) + offset;

	ImVec2 prev_point = bottom2d;
	for (float phi = 0; phi <= 360; phi += 10.f) {
		auto lol1 = DegreesToRadians(phi);
		Eigen::Vector4f next3d = vp * Eigen::Vector4f{ xFlt + rFlt * cos(lol1), yFlt + rFlt * sin(lol1), zFlt,  1.f };
		ImVec2 next2d = ImVec2(next3d(0) / next3d(3), next3d(1) / next3d(3)) + offset;
		dl->AddLine(prev_point, next2d, IM_COL32(_color.At(0), _color.At(1), _color.At(2), 255));
		prev_point = next2d;
	}

	prev_point = bottom2d;
	for (float phi = 0; phi <= 360; phi += 10.f) {
		auto lol1 = DegreesToRadians(phi);
		Eigen::Vector4f next3d = vp * Eigen::Vector4f{ xFlt + rFlt * cos(lol1), yFlt, zFlt + rFlt * sin(lol1),  1.f };
		ImVec2 next2d = ImVec2(next3d(0) / next3d(3), next3d(1) / next3d(3)) + offset;
		dl->AddLine(prev_point, next2d, IM_COL32(_color.At(0), _color.At(1), _color.At(2), 255));
		prev_point = next2d;
	}
}

bool Sphere::TestIntersection(const Ray<double>& ray, HVec<double>& intersection, HVec<double>& localNormal, HVec<double>& localColor) {
	// MOVE RAY TO LOCAL COORDS OF SPHERE
	Ray<double> bRay = _tform.Transform(ray, false);

	// solving the quadratic equation
	HVec<double> ndir = bRay.direction; ndir.Normalize();
	// a = 1 bcuz magic (a = squared len of unit vector)
	double b = 2.0 * HVec<double>::dot(bRay.p1, ndir);
	double c = HVec<double>::dot(bRay.p1, bRay.p1) - 1.0;
	double D = b * b - 4.0 * c;
	if (D <= 0.0) return false;
	double dSqrt = sqrtf(D);
	double t1 = (-b + dSqrt) * 0.5;
	double t2 = (-b - dSqrt) * 0.5;
	if ((t1 < 0.0) || (t2 < 0.0)) return false;

	HVec<double> tempIntersection;
	if (t1 < t2) {
		tempIntersection = bRay.p1 + ndir * t1;
	}
	else {
		tempIntersection = bRay.p1 + ndir * t2;
	}
	// MOVE INTERSECTION TO GLOBAL COORDS
	intersection = _tform.Transform(tempIntersection, true);

	// MOVE CENTRE TO GLOBAL COORDS AND CALC NORMAL!
	localNormal = intersection - _tform.Transform(HVec<double> {0.0, 0.0, 0.0}, true);
	localNormal.Normalize();
	localColor = _color;
	return true;
}