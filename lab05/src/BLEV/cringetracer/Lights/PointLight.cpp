#include "../headers/cringetracer/Lights/PointLight.h"
#include "../headers/cringetracer/GeometricBodies/LightSphere.h"
#include "../headers/cringetracer/Materials/Material.h"

#define PIDIVTWO 1.5708

PointLight::PointLight()
{
	position = HVec<double>{ 0.0, 0.0, 0.0 };
	intensity = 1.0;
	LightSource = new LightSphere(position, 0.1, diffuse * specular);
}

PointLight::PointLight(const HVec<double>& inPosition)
{
	position = inPosition;
	LightSource = new LightSphere(position, 0.1, diffuse * specular);
}

PointLight::PointLight(const HVec2<double>& inPitchYaw, const double inR)
{
	r = inR;
	updatePitchYaw(inPitchYaw);
	LightSource = new LightSphere(position, 0.1, diffuse * specular);
}

PointLight::PointLight(const HVec<double>& inPosition,
	const HVec<double>& inAmbient,
	const HVec<double>& inDiffuse,
	const HVec<double>& inSpecular,
	const double inIntensity) {
	position = inPosition;

	ambient = inAmbient;
	diffuse = inDiffuse;
	specular = inSpecular;

	intensity = inIntensity;
	LightSource = new LightSphere(position, 0.1, diffuse * specular);
}

PointLight::PointLight(const HVec2<double>& inPitchYaw, const double inR,
	const HVec<double>& inAmbient,
	const HVec<double>& inDiffuse,
	const HVec<double>& inSpecular,
	const double inIntensity) {
	r = inR;
	updatePitchYaw(inPitchYaw);

	ambient = inAmbient;
	diffuse = inDiffuse;
	specular = inSpecular;

	intensity = inIntensity;
	LightSource = new LightSphere(position, 0.1, diffuse * specular);
}

PointLight::PointLight(const HVec2<double>& inPitchYaw, const double inR,

	const ImVec3& inAmbient,
	const ImVec3& inDiffuse,
	const ImVec3& inSpecular,

	const double inIntensity)
{
	r = inR;
	updatePitchYaw(inPitchYaw);

	ambient = HVec<double>(inAmbient);
	diffuse = HVec<double>(inDiffuse);
	specular = HVec<double>(inSpecular);

	intensity = inIntensity;
	LightSource = new LightSphere(position, 0.1, diffuse * specular);
}

PointLight::~PointLight()
{
	delete LightSource;
}

bool PointLight::ComputeLighting(
	const HVec<double>& intersection, const HVec<double>& localNormal, const GeometricBody* gb,
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
	GeometricBody* lightBlocker = nullptr;
	for (auto& body : bodies) {
		if (body == gb) continue;
		if (!body->Show) continue;
		if (dynamic_cast<LightSphere*>(body) != nullptr) continue; // 1) light source body does not cast shadow

		if (!(foundLightBlocker = body->TestIntersection(rayToLight, poi, poiNormal, poiColor))) continue;
		if ((poi - start).len() > lightDist) foundLightBlocker = false;
		if (foundLightBlocker) {
			lightBlocker = body;
			break;
		}
	}

	if (foundLightBlocker) {
		//outColor = color;
		outIntensity = 0.0;
		return false;
	}

	outIntensity = intensity * std::max(HVec<double>::dot(localNormal, lightDir), 0.0);
	//outColor = color;
	return outIntensity > 0.0;
}