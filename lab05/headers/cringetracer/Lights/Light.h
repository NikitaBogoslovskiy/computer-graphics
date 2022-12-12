#pragma once
#include "../HeyraMath/HVec.h"
#include "../GeometricBodies/GeometricBody.h"

class Light
{
public:
	Light();
	~Light();

	// todo f: pyr -> position
	HVec<double> pyr;
	HVec<double> position;
	HVec<double> color;
	double intensity;

	virtual bool Illuminate(const HVec<double>& intersection,
		const HVec<double>& localNormal,
		const GeometricBody* gb,
		const std::vector<GeometricBody*>& bodies,
		HVec<double>& outColor, double& outIntensity) = 0;

private:

};
