#pragma once
#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "pch.h"
#include "funcs.h"
#include "../headers/geometry/primitives3d/mesh.h"
#include "../headers/geometry/primitives3d/sphere.h"

// point of light
struct POL {
public:
	ImVec3 pos;

	ImVec3 i_a = { 1.f, 1.f , 1.f }; // мощность фонового освещения
	ImVec3 i_d = { 1.f, 1.f , 1.f }; // мощность рассеянного освещения
	ImVec3 i_s = { 1.f, 1.f , 1.f }; // мощность зеркального освещения

	//ImVec3 attenuation = { 1.0f, 0.0014f, 0.000007f };
	ImVec3 attenuation = { 0.77f, 0.0005f, 0.000003f };

	inline float att(float d) const {
		return 1.f / (attenuation[0] + attenuation[1] * d + attenuation[2] * d * d);
	}
	
	float radius = 20.f; // visual

	inline float length_from(const ImVec3& sp, const ImVec3& rayDir) const {
		ImVec3 v = sp - pos;

		float b = 2.0f * dot_product(rayDir, v);
		float c = dot_product(v, v) - (radius * radius);

		float discriminant = powf(b, 2.f) - 4.0f * c;

		if (discriminant <= 0.0f)
			return FLT_MAX;
		
		float closestT = (-b - sqrtf(discriminant)) / 2.0f;

		if (closestT < 0.f) return FLT_MAX;

		return closestT;
	}

};

class Raytracing {
	using wsize = struct { int width, height; };
	std::vector<ImVec3> rays;
	wsize size;
	ImVec2 offset;

	const std::vector<POL*>* pols;

	bool to_clear = false;

	using objects = struct {
		std::vector<Mesh*>& meshes;
		std::vector<Sphere*>& spheres;
	};
	static constexpr float eps = 0.1f;

	int threadnum = 16;
	std::vector<std::pair<int, int>> bounds;

public:
	Raytracing();
	Raytracing(const std::vector<POL*>* _pols);
	Raytracing(int width, int height, const ImVec2& offset, const std::vector<POL*>* _pols);
	void clear();
	void resize(int width, int height);
	void setOffset(const ImVec2& new_offset);
	void draw(ImDrawList* draw_list, const ImVec2& offset);
	void render(objects objects, Camera& cam);
private:
	ImVec3 renderUnit(const ImVec3& sp, const ImVec3& rayDir, objects objects, bool insideMesh, int8_t last_bounces);
};

#endif RAYTRACING_H