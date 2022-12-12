#include "../headers/cringetracer/Lights/PointLight.h"

#define PIDIVTWO 1.5708

PointLight::PointLight()
{
	color = HVec<double>{ 1.0, 1.0, 1.0 };
	intensity = 1.0;
}

PointLight::PointLight(const HVec<double>& inPitchYaw, const double inR, const HVec<double>& inColor, const double inIntensity)
{
	r = inR;
	updatePitchYaw(inPitchYaw);
	color = inColor;
	intensity = inIntensity;
}

PointLight::~PointLight()
{
}

bool PointLight::Illuminate(const HVec<double>& intersection,
	const HVec<double>& localNormal,
	const GeometricBody* gb,
	const std::vector<GeometricBody*>& bodies,
	HVec<double>& outColor, double& outIntensity)
{
	HVec<double> lightDir = (position - intersection);
	double lightDist = lightDir.len();
	lightDir.Normalize();

	HVec<double> start = intersection;
	Ray<double> rayToLight(start, start + lightDir);

	HVec<double> curInt(3); HVec<double> curLocalNormal(3); HVec<double> curLocalColor(3);
	bool foundLightBlocker = false;
	for (auto& body : bodies) {
		if (body == gb) continue;
		if (foundLightBlocker = body->TestIntersection(rayToLight, curInt, curLocalNormal, curLocalColor)) break;
		if ((curInt - start).len() > lightDist) foundLightBlocker = false;
	}

	if (foundLightBlocker) {
		outColor = color;
		outIntensity = 0.0;
		return false;
	}

	// <(localNormal, lightRay)
	double angle = acos(HVec<double>::dot(localNormal, lightDir)); // local normal is unit
	// normal pointing away from light: no illumination
	if (angle > PIDIVTWO) {
		outColor = color;
		outIntensity = 0.0;
		return false;
	}
	outColor = color;
	// intensity is linearly proportional to the angle between the normal and direction of the light for now
	outIntensity = intensity * (1.0 - (angle / PIDIVTWO));
	return true;
}