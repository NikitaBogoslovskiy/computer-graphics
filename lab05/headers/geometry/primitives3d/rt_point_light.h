#pragma once

#include "rtobject.h"

class RTPointLight : public RTEntity
{
public:
	ImVec3 center;
	float radius;
	float intensity;
	float attenuation;
	ImVec4 color;
	float max_distance;
	float attenuation_coefficient;
	RTPointLight(const ImVec3& _center, float _radius)
	{
		center = _center;
		radius = _radius;
		intensity = 0.5f;
		attenuation = 0.0f;
		color = ImVec4(1, 1, 1, 1);
		max_distance = 1000;
		attenuation_coefficient = 0.0f;
	}

	RTPointLight(const ImVec3& _center, float _radius, float _intensity, const ImVec4& _color)
	{
		center = _center;
		radius = _radius;
		intensity = _intensity;
		color = _color;
	}

	//float& getRadius() { return radius; }
	//float& getIntensity() { return intensity; }
	//ImVec3& getCenter() { return center; }
	//ImVec4& getColor() { return color; }
	void computeAttenuation(float distance_to_object) { attenuation = std::min(1.f, distance_to_object / (max_distance * (1 - attenuation_coefficient))); }

	//void setRadius(float& _radius) { radius = _radius; }
	//void setIntensity(float& _intensity) { intensity = _intensity; }
	//void setCenter(const ImVec3& _center) { center = _center; }
	//void setColor(const ImVec4& _color) { color = _color; }

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
