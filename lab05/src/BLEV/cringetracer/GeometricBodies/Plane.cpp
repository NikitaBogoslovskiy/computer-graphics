#include "../headers/cringetracer/GeometricBodies/Plane.h"
#include "../headers/cringetracer/HeyraMath/HMath.h"

Plane::Plane(const HVec<double>& origin, const HVec<double>& rotation, const HVec<double>& scale, const ImVec3& inColor) : GeometricBody(inColor)
{
	this->SetTransform(origin, rotation, scale);
}

Plane::~Plane()
{

}

void Plane::Draw(ImDrawList* dl, const ImVec2& offset, const Eigen::Matrix4f& vp)
{

}

// boost::optional! aaa
bool Plane::TestIntersection(const Ray<double>& ray, HVec<double>& intersection,
	HVec<double>& localNormal,
	HVec<double>& localColor)
{
	// MOVE RAY TO LOCAL COORDS OF PLANE
	Ray<double> localRay = _tform.Transform(ray, false);
	HVec<double> k = localRay.direction.Normalized();
	if (HMath<double>::CloseEnough(k.At(2), 0.0)) return false; // checking if localRay is parallel to the plane xy in which our plain piece lies (in local coords!)

	// localRay = u * [1 0 0] + v * [0 1 0] - tk
	double t = localRay.p1.At(2) / -k.At(2);
	if (t <= 0.0) return false; // intersection is behind the camera

	double u = localRay.p1.At(0) + k.At(0) * t;
	double v = localRay.p1.At(1) + k.At(1) * t;
	if ((std::abs(u) >= 1.0) || (std::abs(v) >= 1.0)) return false;

	intersection = _tform.Transform(localRay.p1 + t * k, true);

	localNormal = _tform.TransformNormal(HVec<double> {0.0, 0.0, -1.0}).Normalized(); //- _tform.Transform(HVec<double> {0.0, 0.0, 0.0}, true))

	localColor = _color;

	return true;
}
