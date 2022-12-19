#include "../headers/cringetracer/Lights/Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::updateOriginDistance(const double R)
{
	if (R < 0.0) return;
	HVec<double> direction = (position - HVec<double>()).Normalized();
	position = R * direction;
	r = R; //caching
	if (HasLightSource())
		UpdateLightSourcePosition();
}

void Light::updatePitchYaw(const HVec2<double>& inPitchYaw)
{

	pitchYaw.SetAt(0, std::min(std::max(inPitchYaw.At(0), -90.0), 90.0));

	pitchYaw.SetAt(1, inPitchYaw.At(1) + (
		(pitchYaw.At(1) >= 360.0) ? -360.0 : 0
		));

	rotateSource();
	if (HasLightSource())
		UpdateLightSourcePosition();
}

void Light::rotateSource()
{
	auto pitchRad = DegreesToRadians(this->pitchYaw.At(0));
	auto yawRad = DegreesToRadians(this->pitchYaw.At(1));
	auto cosPitch = cos(pitchRad);
	position.SetAt(0, r * cosPitch * cos(yawRad));
	position.SetAt(1, r * sin(pitchRad));
	position.SetAt(2, r * cosPitch * sin(yawRad));
	//printf("%lf %lf %lf\n", position.At(0), position.At(1), position.At(2));
}

// all about light source body

bool Light::HasLightSource()
{
	return LightSource != nullptr;
}

void Light::UpdateLightSourcePosition()
{
	LightSource->Origin.SetAt(0, position.x);
	LightSource->Origin.SetAt(1, position.y);
	LightSource->Origin.SetAt(2, position.z);
	LightSource->SetTransform();
}

void Light::UpdateAmbient(const ImVec3& inColor)
{
	ambient = HVec<double>{ inColor.x, inColor.y, inColor.z };
}

void Light::UpdateDiffuse(const ImVec3& inColor)
{
	diffuse = HVec<double>{ inColor.x, inColor.y, inColor.z };
	LightSource->SetColor(diffuse * specular);
}

void Light::UpdateSpecular(const ImVec3& inColor)
{
	specular = HVec<double>{ inColor.x, inColor.y, inColor.z };
	LightSource->SetColor(diffuse * specular);
}

void Light::UpdateIntensity(const double inIntensity)
{
	intensity = inIntensity;
	LightSource->SetColor(diffuse * specular * intensity);
}

