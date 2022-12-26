#pragma once

#include "geometry/primitives3d/rtobject.h"
#include "geometry/primitives3d/rt_point_light.h"
#include "geometry/primitives3d/rt_scene_box.h"
#include "geometry/primitives3d/rt_ambient_light.h"
#include "geometry/primitives3d/rtsphere.h"

#define MAX_DEPTH 3

class RTBuffer
{
	ColorMatrix colorBuffer;
	ImVec2 offset;
public:
	RTBuffer() {}
	RTBuffer(size_t width, size_t height, ImVec2& offset);
	void clear();
	void resize(size_t width, size_t height);
	void setOffset(ImVec2& new_offset);
	void fillBuffers(std::vector<RTEntity*>& rt_entities, const ImVec3& direction, const ImVec3& up, const ImVec3& eye, const ImVec3& target, const ImVec2& size);
	void draw(ImDrawList* draw_list, ImVec2& canvasOffset);
private:
	ImVec4 trace(Ray& ray, std::vector<RTEntity*>& rt_entities, float min_mu, float max_mu, int depth = 0);
	bool findClosest(Ray& ray, std::vector<RTEntity*>& rt_entities, float min_mu, float max_mu, ImVec3& point, ImVec3& normal, RTObject*& object, ImVec4& output_color);
	ImVec4 computeLocalIllumination(std::vector<RTEntity*>& rt_entities, RTObject* object, Ray& ray, ImVec3& point, ImVec3& normal);
	ImVec4 computeReflection(std::vector<RTEntity*>& rt_entities, int depth, Ray& ray, ImVec3 point, ImVec3 normal);
	ImVec4 computeRefraction(std::vector<RTEntity*>& rt_entities, int depth, Ray& ray, RTObject* object, ImVec3 point, ImVec3 normal, float ior);
	inline float mix_floats(const float& a, const float& b, const float& mix) { return b * mix + a * (1 - mix); }
};

