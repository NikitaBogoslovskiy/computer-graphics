#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	//PointLight(const HVec<double>& inPosition, const HVec<double>& inColor, const double& inIntensity);
	PointLight(const HVec<double>& inPitchYaw, const double inR, const HVec<double>& inColor, const double inIntensity);
	~PointLight();

	bool Illuminate(const HVec<double>& intersection,
		const HVec<double>& localNormal,
		const GeometricBody* gb,
		const std::vector<GeometricBody*>& bodies,
		HVec<double>& outColor, double& outIntensity) override;

private:

};
