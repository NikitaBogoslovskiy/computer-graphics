#include "../headers/cringetracer/GeometricBodies/Ellipsoid.h"

Ellipsoid::Ellipsoid(const double x0, const double y0, const double z0,
	const double rX, const double rY, const double rZ,
	const ImVec3& inColor)
	: GeometricBody(HVec<double>{x0, y0, z0}, HVec<double>{ 0.0, 0.0, 0.0 }, HVec<double>{ rX, rY, rZ }, inColor)
{
	//_centre = HVec<double>{ x0, y0, z0 };
	//_rx = rX; _ry = rY; _rz = rZ;
}

Ellipsoid::~Ellipsoid() {}
const double& Ellipsoid::RadiusX() { return Scale.At(0); }
const double& Ellipsoid::RadiusY() { return Scale.At(1); }
const double& Ellipsoid::RadiusZ() { return Scale.At(2); }

//const HVec<double>& Ellipsoid::Centre() {
//	return Origin;
//}

void Ellipsoid::Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp) {
	float rxFlt = static_cast<float> (RadiusX()); // help
	float ryFlt = static_cast<float> (RadiusY());
	float rzFlt = static_cast<float> (RadiusZ());
	float xFlt = static_cast<float> (Origin.At(0));
	float yFlt = static_cast<float> (Origin.At(1));
	float zFlt = static_cast<float> (Origin.At(2));
	Eigen::Vector4f bottom3d = vp * Eigen::Vector4f{ static_cast<float> (Origin.At(0) + rxFlt),  static_cast<float> (Origin.At(1)),  static_cast<float> (Origin.At(2)),  1.f };
	ImVec2 bottom2d = ImVec2(bottom3d(0) / bottom3d(3), bottom3d(1) / bottom3d(3)) + offset;

	ImVec2 prev_point = bottom2d;
	for (float phi = 0; phi <= 360; phi += 10.f) {
		auto lol1 = DegreesToRadians(phi);
		Eigen::Vector4f next3d = vp * Eigen::Vector4f{ xFlt + rxFlt * cos(lol1), yFlt + ryFlt * sin(lol1), zFlt,  1.f };
		ImVec2 next2d = ImVec2(next3d(0) / next3d(3), next3d(1) / next3d(3)) + offset;
		dl->AddLine(prev_point, next2d, IM_COL32(_color.At(0), _color.At(1), _color.At(2), 255));
		prev_point = next2d;
	}

	prev_point = bottom2d;
	for (float phi = 0; phi <= 360; phi += 10.f) {
		auto lol1 = DegreesToRadians(phi);
		Eigen::Vector4f next3d = vp * Eigen::Vector4f{ xFlt + rxFlt * cos(lol1), yFlt, zFlt + rzFlt * sin(lol1),  1.f };
		ImVec2 next2d = ImVec2(next3d(0) / next3d(3), next3d(1) / next3d(3)) + offset;
		dl->AddLine(prev_point, next2d, IM_COL32(_color.At(0), _color.At(1), _color.At(2), 255));
		prev_point = next2d;
	}
}

bool Ellipsoid::TestIntersection(const Ray<double>& ray, HVec<double>& intersection, HVec<double>& localNormal, HVec<double>& localColor) const {
	// MOVE RAY TO LOCAL COORDS OF SPHERE
	Ray<double> localRay = _tform.Transform(ray, false);

	// solving the quadratic equation
	HVec<double> ndir = localRay.direction.Normalized();
	// a = 1 bcuz magic (a = squared len of unit vector)
	double b = 2.0 * HVec<double>::dot(localRay.p1, ndir);
	double c = HVec<double>::dot(localRay.p1, localRay.p1) - 1.0;
	double D = b * b - 4.0 * c;
	if (D <= 0.0) return false;
	double dSqrt = sqrtf(D);
	double t1 = (-b + dSqrt) * 0.5;
	double t2 = (-b - dSqrt) * 0.5;
	if ((t1 < 0.0) && (t2 < 0.0)) return false; // part of body is behind eye

	HVec<double> poi;
	if (t1 < t2) {
		if (t1 > 0.0)
			poi = localRay.p1 + ndir * t1;
		else if (t2 > 0.0)
			poi = localRay.p1 + ndir * t2;
		else
			return false;
	}
	else {
		if (t2 > 0.0)
			poi = localRay.p1 + ndir * t2;
		else if (t1 > 0.0)
			poi = localRay.p1 + ndir * t1;
		else
			return false;
	}
	// MOVE INTERSECTION TO GLOBAL COORDS
	intersection = _tform.Transform(poi, true);

	// MOVE CENTRE TO GLOBAL COORDS AND CALC NORMAL!
	//localNormal = (intersection - _tform.Transform(HVec<double> {0.0, 0.0, 0.0}, true)).Normalized();
	localNormal = _tform.TransformNormal(poi).Normalized();

	localColor = _color;
	return true;
}