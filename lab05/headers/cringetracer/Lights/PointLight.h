#pragma once
#include "Light.h"

class PointLight : public Light
{
	//HVec<double> _att{ 0.6, 0.00007, 0.0000014 };
	HVec<double> _att{ 1.0,	0.0014,	0.000007 };
public:

	inline const HVec<double>& GetAttenuation() { return _att; }
	inline void SetAttenuation(const size_t ind, const double value) {
		if ((ind < 0) || (ind > 2)) return;
		double _dot = 0;
		for (size_t i = 0; i < 3; i++) {
			_dot += (i == ind) ? value * value : _att[i] * _att[i];
		}
		if (_dot == 0.0) return;
		_att[ind] = value;
	}

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
