#include "cringetracer/GeometricBodies/Cone.h"
#include "cringetracer/HeyraMath/HMath.h"

Cone::Cone(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor)
	: GeometricBody(origin, rotation, scale, inColor)
{
	//_rx = scale.At(0); _h = scale.At(1) * 2.0; _rz = scale.At(2);
}

Cone::~Cone()
{
}

void Cone::Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp)
{
}

bool Cone::TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) const
{
	Ray<double> localRay = _tform.Transform(ray, false);
	HVec<double> v = localRay.direction.Normalized();
	HVec<double> p = localRay.p1;

	// quadratic equation to gain t
	double a = std::pow(v.At(0), 2.0) + std::pow(v.At(1), 2.0) - std::pow(v.At(2), 2.0);
	double b = 2 * (p.At(0) * v.At(0) + p.At(1) * v.At(1) - p.At(2) * v.At(2));
	double c = std::pow(p.At(0), 2.0) + std::pow(p.At(1), 2.0) - std::pow(p.At(2), 2.0);
	double D = std::pow(b, 2.0) - 4 * a * c;

	std::array<double, 3> t;
	std::array<HVec<double>, 3> poi;
	std::array<bool, 3> landed;
	// checking if ray lands onto cylindric surface
	if (D <= 0.0) {
		landed.at(0) = false;
		t.at(0) = std::numeric_limits<double>::max();
		landed.at(1) = false;
		t.at(1) = std::numeric_limits<double>::max();
	}
	else {
		D = std::sqrt(D);
		t.at(0) = (-b + D) / (2 * a);
		t.at(1) = (-b - D) / (2 * a);

		// calc intersections
		poi.at(0) = p + v * t[0];
		poi.at(1) = p + v * t[1];

		landed.at(0) = (t.at(0) > 0.0) && (poi.at(0).At(2) > 0.0) && (poi.at(0).At(2) < 1.0);
		if (!landed.at(0)) t.at(0) = std::numeric_limits<double>::max();

		landed.at(1) = (t.at(1) > 0.0) && (poi.at(1).At(2) > 0.0) && (poi.at(1).At(2) < 1.0);
		if (!landed.at(1)) t.at(1) = std::numeric_limits<double>::max();
	}

	// checking if ray lands onto caps
	if (HMath<double>::CloseEnough(v.At(2), 0.0))
	{
		landed.at(2) = false;
		t.at(2) = std::numeric_limits<double>::max();
	}
	else
	{
		t.at(2) = (p.At(2) - 1.0) / -v.At(2);

		// calc intersections
		poi.at(2) = p + t.at(2) * v;

		landed.at(2) = (t.at(2) > 0.0) && (std::sqrt(std::pow(poi.at(2).At(0), 2.0) + std::pow(poi.at(2).At(1), 2.0)) < 1.0);
		if (!landed.at(2)) t.at(2) = std::numeric_limits<double>::max();
	}

	if (!(landed.at(0) || landed.at(1) || landed.at(2))) return false;

	// Check for the smallest valid value of t.
	size_t minIndex = 0;
	double minValue = 10e6;
	for (size_t i = 0; i < 3; ++i)
	{
		if (t.at(i) >= minValue) continue;
		minValue = t.at(i);
		minIndex = i;
	}

	HVec<double> tPoi = poi.at(minIndex);
	if (minIndex < 2) // intersected cylindric surface
	{
		// local normal goes from pythagoras theorem
		localNormal = _tform.TransformNormal(HVec<double> { tPoi.At(0), tPoi.At(1), -std::sqrt(std::pow(tPoi.At(0), 2.0) + std::pow(tPoi.At(1), 2.0)) }).Normalized(); //- _tform.Transform(HVec<double> { 0.0, 0.0, 0.0 }, true))
	}
	else
	{
		if (HMath<double>::CloseEnough(v.At(2), 0.0)) return false;
		if (std::sqrt(std::pow(tPoi.At(0), 2.0) + std::pow(tPoi.At(1), 2.0)) >= 1.0) return false;
		localNormal = _tform.TransformNormal(HVec<double> { 0.0, 0.0, 1.0 }).Normalized(); //- _tform.Transform(HVec<double> { 0.0, 0.0, 0.0 }, true))
	}

	intPoint = _tform.Transform(tPoi, true);
	localColor = _color;

	return true;
}

