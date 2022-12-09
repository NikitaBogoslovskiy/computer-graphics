#pragma once
#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "pch.h"
#include "funcs.h"
#include "../headers/geometry/primitives3d/mesh.h"
#include "../headers/geometry/primitives3d/sphere.h"

// point of light
struct POL {
	ImVec3 pos;

	ImVec3 i_a = { 1.f, 1.f , 1.f }; // мощность фонового освещения
	ImVec3 i_d = { 1.f, 1.f , 1.f }; // мощность рассеянного освещения
	ImVec3 i_s = { 1.f, 1.f , 1.f }; // мощность зеркального освещения

	//ImVec3 attenuation = { 1.0f, 0.0014f, 0.000007f };
	ImVec3 attenuation = { 0.77f, 0.0005f, 0.000003f };

	inline float att(float d) {
		return 1.f / (attenuation[0] + attenuation[1] * d + attenuation[2] * d * d);
	}
	// color = white
};

class Raytracing {
	using wsize = struct { int width, height; };
	std::vector<ImVec3> rays;
	wsize size;
	ImVec2 offset;

	std::vector<POL> pols;

	bool to_clear = false;

	using objects = struct {
		std::vector<Mesh*>& meshes;
		std::vector<Sphere*>& spheres;
	};

	static constexpr float eps = 0.00001f;

public:
	Raytracing();
	Raytracing(int width, int height, const ImVec2& offset);
	void clear();
	void resize(int width, int height);
	void setOffset(const ImVec2& new_offset);
	void draw(ImDrawList* draw_list, const ImVec2& offset);
	void render(objects objects, Camera& cam);
private:
	ImVec3 renderUnit(const ImVec3& sp, const ImVec3& rayDir, objects objects, bool insideMesh, int8_t last_bounces);
};

#endif RAYTRACING_H