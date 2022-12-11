#include "cringetracer/GeometricBodies/Cylinder.h"
#include "cringetracer/HeyraMath/HMath.h"

Cylinder::Cylinder(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor) : GeometricBody(inColor)
{
	this->SetTransform(origin, rotation, scale);
	_rx = scale.At(0); _h = scale.At(1) * 2.0; _rz = scale.At(2);
}

Cylinder::~Cylinder()
{
}

void Cylinder::Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp)
{
}

bool Cylinder::TestIntersection(const Ray<double>& ray, HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor)
{
	Ray<double> localRay = _tform.Transform(ray, false);
	HVec<double> v = localRay.direction.Normalized();
	HVec<double> p = localRay.p1;

	// quadratic equation to gain t
	double a = std::pow(v.At(0), 2.0) + std::pow(v.At(1), 2.0);
	double b = 2.0 * (p.At(0) * v.At(0) + p.At(1) * v.At(1));
	double c = std::pow(p.At(0), 2.0) + std::pow(p.At(1), 2.0) - 1.0;
	double D = std::pow(b, 2.0) - 4 * a * c;

	std::array<double, 4> t;
	std::array<HVec<double>, 4> poi;
	std::array<bool, 4> landed;
	// checking if ray lands onto cylindric surface
	if (D <= 0.0) {
		landed.at(0) = false;
		landed.at(1) = false;
		t.at(0) = std::numeric_limits<double>::max();
		t.at(1) = std::numeric_limits<double>::max();
	}
	else {
		D = std::sqrt(D);
		t.at(0) = (-b + D) / (2 * a);
		t.at(1) = (-b - D) / (2 * a);

		// calc intersections
		poi.at(0) = localRay.p1 + (v * t[0]);
		poi.at(1) = localRay.p1 + (v * t[1]);

		landed.at(0) = (t.at(0) > 0.0) && (std::abs(poi.at(0).At(2)) < 1.0); // we only interested 
		if (!landed.at(0)) t.at(0) = std::numeric_limits<double>::max();

		landed.at(1) = (t.at(1) > 0.0) && (std::abs(poi.at(1).At(2)) < 1.0);
		if (!landed.at(1)) t.at(1) = std::numeric_limits<double>::max();
	}

	// And test the end caps.
	if (HMath<double>::CloseEnough(v.At(2), 0.0))
	{
		landed.at(2) = false;
		t.at(2) = std::numeric_limits<double>::max();

		landed.at(3) = false;
		t.at(3) = std::numeric_limits<double>::max();
	}
	else
	{
		t.at(2) = (localRay.p1.At(2) - 1.0) / -v.At(2);
		t.at(3) = (localRay.p1.At(2) + 1.0) / -v.At(2);

		// calc intersections
		poi.at(2) = localRay.p1 + t.at(2) * v;
		poi.at(3) = localRay.p1 + t.at(3) * v;

		landed.at(2) = (t.at(2) > 0.0) && (std::sqrt(std::pow(poi.at(2).At(0), 2.0) + std::pow(poi.at(2).At(1), 2.0)) < 1.0);
		if (!landed.at(2)) t.at(2) = std::numeric_limits<double>::max();

		landed.at(3) = (t.at(3) > 0.0) && (std::sqrt(std::pow(poi.at(3).At(0), 2.0) + std::pow(poi.at(3).At(1), 2.0)) < 1.0);
		if (!landed.at(3)) t.at(3) = std::numeric_limits<double>::max();
	}

	if (!(landed.at(0) || landed.at(1) || landed.at(2) || landed.at(3))) return false;

	// Check for the smallest valid value of t.
	size_t minIndex = 0;
	double minValue = 10e6;
	for (size_t i = 0; i < 4; ++i)
	{
		if (t.at(i) >= minValue) continue;
		minValue = t.at(i);
		minIndex = i;
	}

	HVec<double> validPOI = poi.at(minIndex);
	if (minIndex < 2) // intersected cylindric surface
	{
		localNormal = (_tform.Transform(HVec<double> { validPOI.At(0), validPOI.At(1), 0.0 }, true) - _tform.Transform(HVec<double> { 0.0, 0.0, 0.0 }, true))
			.Normalized();
	}
	else
	{
		if (HMath<double>::CloseEnough(v.At(2), 0.0)) return false;
		if (std::sqrt(std::pow(validPOI.At(0), 2.0) + std::pow(validPOI.At(1), 2.0)) >= 1.0) return false;
		localNormal = (_tform.Transform(HVec<double> { 0.0, 0.0, validPOI.At(2) }, true) - _tform.Transform(HVec<double> { 0.0, 0.0, 0.0 }, true))
			.Normalized();
	}

	intPoint = _tform.Transform(validPOI, true);
	localColor = _color;

	return true;
}
