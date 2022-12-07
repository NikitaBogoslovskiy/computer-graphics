#include "../headers/cringetracer/Lights/PointLight.h"

#define PIDIVTWO 1.5708

PointLight::PointLight(const HVec<double>& inPosition, const HVec<double>& inColor, const double& inIntensity)
{
	color = inColor;
	position = inPosition;
	intensity = inIntensity;
}


PointLight::PointLight()
{
	color = HVec<double>{ 1.0, 1.0, 1.0 };
	intensity = 1.0;
}

PointLight::~PointLight()
{
}

bool PointLight::Illuminate(const HVec<double>& intersection,
	const HVec<double>& localNormal,
	const GeometricBody* gb,
	const std::vector<GeometricBody*> bodies,
	HVec<double>& outColor, double& outIntensity)
{
	// light position -> intersection
	HVec<double> lightDir = (position - intersection).Normalized();

	// starting point
	HVec<double> startPoint = intersection;

	// <(localNormal, lightRay)
	double angle = acos(HVec<double>::dot(localNormal, lightDir));

	// normal pointing away from light: no illumination
	if (angle > PIDIVTWO) {
		outColor = color;
		outIntensity = 0.0;
		return false;
	}

	outColor = color;
	// intensity is linearly proportional to the angle between the normal and direction of the light
	outIntensity = intensity * (1.0 - (angle / PIDIVTWO));
	return true;
}