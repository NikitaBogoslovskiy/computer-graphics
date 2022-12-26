#pragma once

#include "rtobject.h"

class RTAmbientLight : public RTEntity
{
public:
	float intensity;
	ImVec4 color;
	RTAmbientLight()
	{
		intensity = 0.1f;
		color = ImVec4(1.f, 1.f, 1.f, 1.f);
	}

	RTAmbientLight(float _intensity, const ImVec4& _color)
	{
		intensity = _intensity;
		color = _color;
	}

	//ImVec4& getColor() { return color; }
	//float& getIntensity() { return intensity; }

	//void setIntensity(float& _intensity) { intensity = _intensity; }
	//void setColor(const ImVec4& _color) { color = _color; }

	virtual bool getNearestIntersection(Ray& ray, float& mu, ImVec3& intersection_point, ImVec3& intersection_normal) override
	{
		return false;
	}
};
