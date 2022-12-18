#pragma once

#include "rtobject.h"

class RTSphere : public RTObject
{
	ImVec3 center;
	float radius;
public:
	RTSphere(const ImVec3& _center, float _radius)
	{
		center = _center;
		radius = _radius;
	}
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
