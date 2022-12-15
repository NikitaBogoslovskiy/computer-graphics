#include "../headers/cringetracer/Lights/PointLight.h"
#include "../headers/cringetracer/GeometricBodies/LightSphere.h"

#define PIDIVTWO 1.5708

PointLight::PointLight()
{
	position = HVec<double>{ 0.0, 0.0, 0.0 };
	color = HVec<double>{ 1.0, 1.0, 1.0 };
	intensity = 1.0;
	LightSource = new LightSphere(position, 1.0, color);
}

PointLight::PointLight(const HVec<double>& inPosition, const HVec<double>& inColor, const double& inIntensity)
{
	position = inPosition;
	color = inColor;
	intensity = inIntensity;
	LightSource = new LightSphere(position, 1.0, color);
}

PointLight::PointLight(const HVec2<double>& inPitchYaw, const double inR, const HVec<double>& inColor, const double inIntensity)
{
	r = inR;
	updatePitchYaw(inPitchYaw);
	color = inColor;
	intensity = inIntensity;
	LightSource = new LightSphere(position, 1.0, color);
}

PointLight::~PointLight()
{
	delete LightSource;
}

bool PointLight::ComputeLighting(const HVec<double>& intersection,
	const HVec<double>& localNormal,
	const GeometricBody* gb,
	const std::vector<GeometricBody*>& bodies,
	HVec<double>& outColor, double& outIntensity)
{
	HVec<double> lightDir = (position - intersection);
	double lightDist = lightDir.len();
	lightDir.Normalize();

	HVec<double> start = intersection + localNormal * 0.01;
	Ray<double> rayToLight(start, start + lightDir);

	HVec<double> poi, poiNormal, poiColor;
	bool foundLightBlocker = false;
	for (auto& body : bodies) {
		if (body == gb) continue;
		//if (dynamic_cast<LightSphere*>(body) != nullptr) continue; // 1) light source body does not cast shadow
		if (!(foundLightBlocker = body->TestIntersection(rayToLight, poi, poiNormal, poiColor))) continue;
		if ((poi - start).len() > lightDist) foundLightBlocker = false;
		if (foundLightBlocker) break;
	}

	if (foundLightBlocker) {
		outColor = color;
		outIntensity = 0.0;
		return false;
	}

	// actually did not notice any visual difference between these two methods. why not try both
	// <(localNormal, lightRay)
	double angle = acos(HVec<double>::dot(localNormal, lightDir)); // local normal is unit
	// normal pointing away from light: no illumination
	if (angle > PIDIVTWO) {
		outColor = color;
		outIntensity = 0.0;
		return false;
	}
	// intensity is linearly proportional to the angle between the normal and direction of the light for now
	outIntensity = intensity * (1.0 - (angle / PIDIVTWO));
	outColor = color;
	return true;

	//outIntensity = intensity * std::max(HVec<double>::dot(localNormal, lightDir), 0.0);
	//outColor = color;
	//return outIntensity != 0.0;
}