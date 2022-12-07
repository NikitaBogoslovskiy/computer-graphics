#include "../headers/cringetracer/Cringetracer.h"


CringeTracer::CringeTracer()
{
	screen_distance = 1.0;
	width = 0.25; // 1.0
	aspect = 1.0; // 16.0 / 9.0
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
	this->eye = HVec<double>(this->cam->eye());
	this->target = HVec<double>(this->cam->target());
	this->up = HVec<double>(this->cam->up());
}

void CringeTracer::UpdateScreenVectors() {
	primaryVec = target - eye; primaryVec.Normalize();

	u = HVec<double>::cross(primaryVec, up); u.Normalize();
	v = HVec<double>::cross(u, primaryVec); v.Normalize();

	centre = eye + screen_distance * primaryVec;

	u *= width;
	v *= (width / this->GetAspect());
}

// x and y are fractions of width and height of virtual screen. 
// x- fraction of U vector, y - fraction of V vector.
// extremes - [-1, 1]; 0,0 - center of the screen
bool CringeTracer::GenerateRay(const float x, const float y, Ray& outRay) {
	// HVec<double> newX = centre + u * x;
	HVec<double> worldPoint = centre + u * x + v * y;

	outRay.p1 = eye;
	outRay.p2 = worldPoint;
	outRay.direction = worldPoint - eye;
	return true;
}

void CringeTracer::SubRender(const size_t start, const size_t end, const size_t xSize, const size_t ySize, const double xFact, const double yFact) {
	HVec<double> intersection(3);
	HVec<double> localNormal(3);
	HVec<double> localColor(3);

	double minDist = 1e6;
	double maxDist = 0.0;
	for (size_t x = start; x < end; x++)
	{
		for (size_t y = 0; y < ySize; y++)
		{
			// Normalize the x and y coordinates.
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;

			// Generate the ray for this pixel (optimize! one recalc per shift)
			GenerateRay(normX, normY, img.rays[x][y]);

			for (auto & body : scene.bodies) {
				bool intersected = body->TestIntersection(img.rays[x][y], intersection, localNormal, localColor);
				if (intersected)
				{
					//printf("intersection: %lf %lf %lf\n", intersection.At(0), intersection.At(1), intersection.At(2));
					// Compute the distance between the camera and the point of intersection.
					double dist = (intersection - img.rays[x][y].p1).len();
					maxDist = std::max(maxDist, dist);
					minDist = std::min(minDist, dist);

					//img.SetPixel(x, y, body->dcol.At(0), body->dcol.At(1), body->dcol.At(2));
					//img.SetPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
					img.SetPixel(x, y, body->dcol.At(0) - ((dist - 9.0) / 0.94605) * body->dcol.At(0), 
						body->dcol.At(1) - ((dist - 9.0) / 0.94605) * body->dcol.At(1),
						body->dcol.At(2) - ((dist - 9.0) / 0.94605) * body->dcol.At(2));
				}
				else
				{
					img.SetPixel(x, y, 0.0, 0.0, 0.0);
				}
			}
			
		}
	}
	printf("min = %lf | max = %lf\n", minDist, maxDist);
}

void CringeTracer::Render() {
	size_t xSize = img.XSize();
	size_t ySize = img.YSize();

	double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
	double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
	/*
	The arguments to the thread function are moved or copied by value.
	If a reference argument needs to be passed to the thread function, it has to be wrapped (e.g., with std::ref or std::cref).
	*/
	std::thread th1(&CringeTracer::SubRender, this, 0, xSize / 4, xSize, ySize, xFact, yFact);
	std::thread th2(&CringeTracer::SubRender, this, xSize / 4, xSize / 2, xSize, ySize, xFact, yFact);
	std::thread th3(&CringeTracer::SubRender, this, xSize / 2, xSize / 4 * 3, xSize, ySize, xFact, yFact);
	std::thread th4(&CringeTracer::SubRender, this, xSize / 4 * 3, xSize, xSize, ySize, xFact, yFact);

	th1.join();
	th2.join();
	th3.join();
	th4.join();
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