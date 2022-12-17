#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "pch.h"
#include "../headers/geometry/methods/funcs.h"

class Sphere : public MaterialParams {
public:
	ImVec3 pos;
	float radius = 30.f;
	Sphere() {}

	std::pair<float, ImVec3> is_intersected_with_ray(const ImVec3& sp, const ImVec3& direction, bool use_normals) const;
	bool is_intersected_with_light(const ImVec3& sp, const ImVec3& direction, bool use_normals, float max_t) const;
};

#endif // !SPHERE_H
