#include "cringetracer/GeometricBodies/Box.h"
#include "cringetracer/HeyraMath/HMath.h"

Box::Box(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor)
	: GeometricBody(origin, rotation, scale, inColor)
{
}

Box::~Box()
{
}

void Box::Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp)
{
}

bool Box::TestIntersection(const Ray<double>& inRay,
	HVec<double>& intPoint, HVec<double>& localNormal, HVec<double>& localColor) const
{
	Ray<double> localRay = _tform.Transform(inRay, false);
	HVec<double> a = localRay.p1;
	HVec<double> k = localRay.direction.Normalized();

	// u, v - coords of ray's landing place on corresponding plane
	// t - corresponding param of the line
	std::array<double, 6> u, v, t;
	if (HMath<double>::CloseEnough(0.0, k.At(2))) { // check for parallelism to xy
		t[0] = std::numeric_limits<double>::max();
		t[1] = std::numeric_limits<double>::max();
	}
	else {
		t[0] = -(a.At(2) - 1.0) / k.At(2);
		t[1] = -(a.At(2) + 1.0) / k.At(2);

		u[0] = a.At(0) + k.At(0) * t[0];
		v[0] = a.At(1) + k.At(1) * t[0];

		u[1] = a.At(0) + k.At(0) * t[1];
		v[1] = a.At(1) + k.At(1) * t[1];
	}

	if (HMath<double>::CloseEnough(0.0, k.At(0))) { // check for parallelism to yz
		t[2] = std::numeric_limits<double>::max();
		t[3] = std::numeric_limits<double>::max();
	}
	else {
		t[2] = -(a.At(0) + 1.0) / k.At(0);
		t[3] = -(a.At(0) - 1.0) / k.At(0);

		u[2] = a.At(2) + k.At(2) * t[2];
		v[2] = a.At(1) + k.At(1) * t[2];

		u[3] = a.At(2) + k.At(2) * t[3];
		v[3] = a.At(1) + k.At(1) * t[3];
	}

	if (HMath<double>::CloseEnough(0.0, k.At(1))) { // check for parallelism to xz
		t[4] = std::numeric_limits<double>::max();
		t[5] = std::numeric_limits<double>::max();
	}
	else {
		t[4] = -(a.At(1) + 1.0) / k.At(1);
		t[5] = -(a.At(1) - 1.0) / k.At(1);

		u[4] = a.At(0) + k.At(0) * t[4];
		v[4] = a.At(2) + k.At(2) * t[4];

		u[5] = a.At(0) + k.At(0) * t[5];
		v[5] = a.At(2) + k.At(2) * t[5];
	}

	bool intersected = false;
	double minT = std::numeric_limits<double>::max();
	int minIndex = 0;

	for (int i = 0; i < 6; ++i)
	{
		if (!((std::abs(u[i]) <= 1.0) && (std::abs(v[i]) <= 1.0)
			&& (t[i] < minT) && (t[i] > 0.0))) continue;
		minIndex = i;
		minT = t[i];
		intersected = true;
	}
	if (!intersected) return false;

	intPoint = _tform.Transform(a + minT * k, true);
	localNormal = _tform.TransformNormal(normals[minIndex]).Normalized();
	localColor = _color;

	return true;
}

