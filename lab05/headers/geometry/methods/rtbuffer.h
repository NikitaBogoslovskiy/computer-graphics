#pragma once

#include "geometry/primitives3d/rtobject.h"
#include "geometry/primitives3d/rt_point_light.h"

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
	ImVec4 trace(Ray& ray, std::vector<RTEntity*>& rt_entities);
};

