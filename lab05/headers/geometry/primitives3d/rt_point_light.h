#pragma once

#include "rtobject.h"

class RTPointLight : public RTEntity
{
	ImVec3 center;
	float radius;
	float intensity;
	ImVec4 color;
public:
	RTPointLight(const ImVec3& _center, float _radius)
	{
		center = _center;
		radius = _radius;
		intensity = 1.0f;
		color = ImVec4(255, 255, 255, 255);
	}

	RTPointLight(const ImVec3& _center, float _radius, float _intensity, const ImVec4& _color)
	{
		center = _center;
		radius = _radius;
		intensity = _intensity;
		color = _color;
	}

	float& getRadius() { return radius; }
	float& getIntensity() { return intensity; }
	ImVec3& getCenter() { return center; }
	ImVec4& getColor() { return color; }

	void getRadius(float& _radius) { radius = _radius; }
	void getIntensity(float& _intensity) { intensity = _intensity; }
	void getCenter(const ImVec3& _center) { center = _center; }
	void getColor(const ImVec4& _color) { color = _color; }

	virtual bool getNearestIntersection(Ray& ray, float& mu, ImVec3& intersection_point, ImVec3& intersection_normal) override
	{
		ImVec3 delta_p = ray.origin - center;
		float dir_dot_delta = ray.direction * delta_p;
		float sqr_addition = pow(dir_dot_delta, 2) - delta_p * delta_p + pow(radius, 2);
		if (sqr_addition < 0)
			return false;
		float sqrt_addition = sqrt(sqr_addition);
		float mu1 = -dir_dot_delta - sqrt_addition;
		float mu2 = -dir_dot_delta + sqrt_addition;
		if (mu1 >= 0)
		{
			mu = mu1;
			intersection_point = ray.origin + mu1 * ray.direction;
			intersection_normal = normilize(intersection_point - center);
			return true;
		}
		if (mu2 >= 0)
		{
			mu = mu2;
			intersection_point = ray.origin + mu2 * ray.direction;
			intersection_normal = normilize(intersection_point - center);
			return true;
		}
		return false;
	}
};
