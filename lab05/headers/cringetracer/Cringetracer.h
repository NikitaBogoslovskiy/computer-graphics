#pragma once
#include "cringetracer/Image.h"
#include "cringetracer/Scene.h"
#include "cringetracer/Ray.h"
#include "cringetracer/HeyraMath/HVec.h"
#include "geometry/primitives3d/camera.h"

class CringeTracer
{
	double screen_distance;
	double width;
	double aspect;

	//well lets see if double precision will help me somehow
	HVec<double> eye;
	HVec<double> target;
	HVec<double> up;
	HVec<double> primaryVec;

	HVec<double> u;
	HVec<double> v;
	HVec<double> centre;
	void DoubleCameraInfo();
	void UpdateScreenVectors();

	inline static size_t _THREADS;

public:

	Camera* cam;
	Scene* scene;

	Image img;
	
	CringeTracer();
	void Update();

	void SetCamera(Camera* _cam);
	void SetOMPThreads();

	void SetScreenDistance(const double _distance);
	void SetWidth(const double _width);
	void SetAspect(const double _aspect);

	const HVec<double>& GetEye();
	const HVec<double>& GetTarget();
	const HVec<double>& GetUp();

	const HVec<double>& GetU();
	const HVec<double>& GetV();
	const HVec<double>& GetScreenCentre();

	double GetScreenDistance();
	double GetWidth();
	double GetAspect();

	// x and y are fractions of width and height of virtual screen. 
	// x- fraction of U vector, y - fraction of V vector.
	// extremes - [-1, 1]; 0,0 - center of the screen
	void GetRayXY(const float x, const float y, Ray<double>& outRay); // tip of the vector we cast from the pinhole through virtual screen
	bool CastRay(const Ray<double>& ray,
		GeometricBody*& closestBody, HVec<double>& closestInt, HVec<double>& closestLocalNormal, HVec<double>& closestLocalColor);
	void Render();
	void SubRender(const size_t start, const size_t end, const size_t xSize, const size_t ySize, const double xFact, const double yFact);
};