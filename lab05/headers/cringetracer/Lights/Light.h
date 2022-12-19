#pragma once
#include "../HeyraMath/HVec.h"
#include "../GeometricBodies/GeometricBody.h"

class Light
{
protected:


public:

	HVec<double> position; // "computed" property

	bool Show = true;

	GeometricBody* LightSource = nullptr;
	Light();
	~Light();

	HVec2<double> pitchYaw{ 0.0, 90.0 };
	double r = 10.0;

	HVec<double> ambient{ 0.1, 0.1, 0.1 };
	HVec<double> diffuse{ 1.0, 1.0, 1.0 };
	HVec<double> specular{ 1.0, 1.0, 1.0 };

	double intensity = 1.0;

	virtual bool ComputeLighting(const HVec<double>& intersection,
		const HVec<double>& localNormal,
		const GeometricBody* gb,
		const std::vector<GeometricBody*>& bodies,
		HVec<double>& outColor, double& outIntensity) = 0;

	// requires r to be set
	// updates pitchYaw and position
	void updatePitchYaw(const HVec2<double>& inPitchYaw);
	void updateOriginDistance(const double R);

	bool HasLightSource();
	void UpdateLightSourcePosition();

	void UpdateAmbient(const ImVec3& inColor);
	void UpdateDiffuse(const ImVec3& inColor);
	void UpdateSpecular(const ImVec3& inColor);

	void UpdateIntensity(const double inIntensity);

private:
	void rotateSource();
};
