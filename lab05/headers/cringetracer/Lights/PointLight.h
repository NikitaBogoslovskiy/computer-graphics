#pragma once
#include "Light.h"

class PointLight : public Light
{
public:

	HVec<double> attenuation{ 0.6, 0.00007, 0.0000014 };

	PointLight();

	PointLight(const HVec<double>& inPosition);
	PointLight(const HVec2<double>& inPitchYaw, const double inR);

	PointLight(const HVec<double>& inPosition,

		const HVec<double>& inAmbient,
		const HVec<double>& inDiffuse,
		const HVec<double>& inSpecular,

		const double inIntensity);

	PointLight(const HVec2<double>& inPitchYaw, const double inR,

		const HVec<double>& inAmbient,
		const HVec<double>& inDiffuse,
		const HVec<double>& inSpecular,

		const double inIntensity);

	PointLight(const HVec2<double>& inPitchYaw, const double inR,

		const ImVec3& inAmbient,
		const ImVec3& inDiffuse,
		const ImVec3& inSpecular,

		const double inIntensity);

	~PointLight();

	bool ComputeLighting(const HVec<double>& intersection,
		const HVec<double>& localNormal,
		const GeometricBody* gb,
		const std::vector<GeometricBody*>& bodies,
		HVec<double>& outColor, double& outIntensity) override;

private:

};
