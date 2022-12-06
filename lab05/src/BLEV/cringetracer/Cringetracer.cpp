#include "../headers/cringetracer/Cringetracer.h"


CringeTracer::CringeTracer()
{
	screen_distance = 1.0;
	width = 1.0;
	aspect = 1.0;
}


void CringeTracer::SetCamera(Camera* _cam) {
	cam = _cam;
	DoubleCameraInfo();
}

void CringeTracer::Update() {
	DoubleCameraInfo();
	UpdateScreenVectors();
}

void CringeTracer::DoubleCameraInfo() {
	eye = HVec<double>(this->cam->eye());
	target = HVec<double>(this->cam->target());
	up = HVec<double>(this->cam->up());
}

void CringeTracer::UpdateScreenVectors() {
	primaryVec = target - eye; primaryVec.Normalize();

	u = HVec<double>::cross(primaryVec, up); u.Normalize();
	v = HVec<double>::cross(u, primaryVec); v.Normalize();

	centre = eye + screen_distance * primaryVec;

	u *= width;
	v *= (width / aspect);
}

// x and y are fractions of width and height of virtual screen. 
// x- fraction of U vector, y - fraction of V vector.
// extremes - [-1, 1]; 0,0 - center of the screen
Ray CringeTracer::GenerateRay(const float x, const float y) {
	HVec<double> newX = centre + u * x;
	HVec<double> worldPoint = newX + v * y;
	return Ray(eye, worldPoint);
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
	return aspect;
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