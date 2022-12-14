#pragma once
#include "../HeyraMath/HVec.h"
#include "../GeometricBodies/GeometricBody.h"

class Light
{
protected:

public:

	GeometricBody* LightSource = nullptr;
	Light();
	~Light();

	HVec2<double> pitchYaw{ 0.0, 90.0 };
	double r = 10.0;
	HVec<double> color{ 1.0, 1.0, 1.0 };
	double intensity = 1.0;

	HVec<double> position; // "computed" property

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
	void UpdateColor(const ImVec3& inColor);
	void UpdateIntensity(const double inIntensity);

private:
	void rotateSource();
};
