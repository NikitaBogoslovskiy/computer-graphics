#include "../headers/cringetracer/Cringetracer.h"
#include "../headers/cringetracer/Materials/Material.h"
#include "../headers/cringetracer/GeometricBodies/LightSphere.h"
#include <omp.h>
#include <algorithm>

CringeTracer::CringeTracer()
{
	screen_distance = 1.0;
	width = 0.25;
	aspect = 1.0;
}

void CringeTracer::SetCamera(Camera* _cam) {
	cam = _cam;
	DoubleCameraInfo();
}

void CringeTracer::SetOMPThreads() {
	_THREADS = std::max(2 * std::thread::hardware_concurrency() / 3, size_t(1));
	printf("THREADS = %lu\n", _THREADS);
	omp_set_num_threads(_THREADS);
}

void CringeTracer::Update() {
	DoubleCameraInfo();
	UpdateScreenVectors();
}

void CringeTracer::DoubleCameraInfo() {
	this->eye = HVec<double>(this->cam->eye());
	this->target = HVec<double>(this->cam->target());
	this->up = HVec<double>(this->cam->up());
}

void CringeTracer::UpdateScreenVectors() {
	primaryVec = (target - eye).Normalized();

	u = HVec<double>::cross(primaryVec, up).Normalized();
	v = HVec<double>::cross(u, primaryVec).Normalized();

	centre = eye + screen_distance * primaryVec;

	u *= width;
	v *= (width / this->GetAspect());
}

// x and y are fractions of width and height of virtual screen. 
// x- fraction of U vector, y - fraction of V vector.
// extremes - [-1, 1]; 0,0 - center of the screen
void CringeTracer::GetRayXY(const float x, const float y, Ray<double>& outRay) {
	HVec<double> worldPoint = centre + u * x + v * y;
	outRay.p1 = eye;
	outRay.p2 = worldPoint;
	outRay.direction = worldPoint - eye;
}

bool CringeTracer::CastRay(const Ray<double>& ray,
	GeometricBody*& closestBody, HVec<double>& closestInt, HVec<double>& closestLocalNormal, HVec<double>& closestLocalColor)
{
	double minDist = std::numeric_limits<double>::max();
	closestBody = nullptr;
	HVec<double> poi, poiNormal, poiColor;
	//HVec<double> intersection, localNormal, localColor;
	for (auto& body : scene.bodies) {
		if (!(body->TestIntersection(ray, poi, poiNormal, poiColor))) continue; // no intersection
		
		double dist = (poi - ray.p1).len();
		if (dist >= minDist) continue;
		minDist = dist;
		closestBody = body;
		closestInt = poi;
		closestLocalNormal = poiNormal;
		closestLocalColor = poiColor;
	}

	return closestBody != nullptr;
}

void CringeTracer::SubRender(const size_t start, const size_t end, const size_t xSize, const size_t ySize, const double xFact, const double yFact) {
	for (size_t x = start; x < end; x++)
	{
		for (size_t y = 0; y < ySize; y++)
		{
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;

			GetRayXY(normX, normY, img.rays[x][y]);

			GeometricBody* closestBody = nullptr;
			HVec<double>  closestIntersection, closestLocalNormal, closestLocalColor;
			if (!(CastRay(img.rays[x][y], closestBody, closestIntersection, closestLocalNormal, closestLocalColor))) continue;

			HVec<double> finalColor;
			if (closestBody->HasMaterial()) {
				finalColor = closestBody->Mtl->ComputeColor(scene.bodies, scene.lights,
					img.rays[x][y],
					closestBody, closestIntersection, closestLocalNormal, 0);
			}
			//else if (dynamic_cast<LightSphere*>(closestBody) == nullptr) {
			else {
				finalColor = Material::ComputeDiffuse(scene.bodies, scene.lights,
					closestBody, closestIntersection, closestLocalNormal, closestBody->GetColor()); // clumsy!!
			}
			//else {
			//	finalColor = 2.0 * closestBody->GetColor(); // 2) light source has no shading
			//	//finalColor = closestBody->GetColor(); // 2) light source has no shading
			//}
			img.SetPixel(x, y, finalColor);
		}
	}
}

void CringeTracer::Render() {
	size_t xSize = img.XSize();
	size_t ySize = img.YSize();
	double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
	double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);

	const size_t chunkSize = xSize / _THREADS;
	//SubRender(0, xSize, xSize, ySize, xFact, yFact);

#pragma omp parallel for
	for (int i = 0; i < _THREADS; ++i)
	{
		SubRender(i * chunkSize, (i + 1) * chunkSize, xSize, ySize, xFact, yFact);
	}
}

// trash ==========================================

void CringeTracer::SetScreenDistance(const double _distance) {
	screen_distance = _distance;
}

void CringeTracer::SetWidth(const double _width) {
	width = _width;
}

void CringeTracer::SetAspect(const double _aspect) {
	aspect = _aspect;
}

const HVec<double>& CringeTracer::GetEye()
{
	return eye;
}
const HVec<double>& CringeTracer::GetTarget()
{
	return target;
}
const HVec<double>& CringeTracer::GetUp()
{
	return up;
}
double CringeTracer::GetScreenDistance()
{
	return screen_distance;
}
double CringeTracer::GetWidth()
{
	return width;
}
double CringeTracer::GetAspect()
{
	return static_cast<double>(img.XSize()) / static_cast<double>(img.YSize());
}
const HVec<double>& CringeTracer::GetU()
{
	return u;
}
const HVec<double>& CringeTracer::GetV()
{
	return v;
}
const HVec<double>& CringeTracer::GetScreenCentre()
{
	return centre;
}