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
	HVec<double> direction = (position - HVec<double> {0.0, 0.0, 0.0}).Normalized();
	position = R * direction;
	r = R; //caching
}

void Light::updatePitchYaw(const HVec<double>& inPitchYaw)
{
	pitchYaw.SetAt(0, std::min(std::max(inPitchYaw.At(0), -89.0), 89.0));

	pitchYaw.SetAt(1, inPitchYaw.At(1) + (
		(pitchYaw.At(1) >= 360.0) ? -360.0 : 0
		));

	rotateSource();
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
