#pragma once
#include "cringetracer/Image.h"
#include "cringetracer/Scene.h"
#include "cringetracer/Ray.h"
#include "cringetracer/HeyraMath/HVec.h"
#include "geometry/primitives3d/camera.h"

class CringeTracer
{

public:
	Camera* cam;
	Image img;
	Scene scene;

	CringeTracer();

	void SetCamera(Camera* _cam);

	//void SetScreenDistance(const double _distance);
	//void SetWidth(const double _width);
	//void SetAspect(const double _width);

	//HVec<double> GetU();
	//HVec<double> GetV();
	//HVec<double> GetScreenCentre();

	//double GetScreenDistance();
	//double GetWidth();
	//double GetAspect();

	//// x and y are fractions of width and height of virtual screen. 
	//// x- fraction of U vector, y - fraction of V vector.
	//// extremes - [-1, 1]; 0,0 - center of the screen
	//Ray GenerateRay(const float x, const float y); // tip of the vector we cast from the pinhole through virtual screen
};
