#pragma once

#include "rotation_body.h"

class Torch : public RotationBody
{
	ImVec3 normal;
	ImVec4 color;
	float intensity;
public:
	Torch(ImVec4 _color = ImVec4(255, 255, 255, 255), float _intensity = 1.0)
	{
		color = _color;
		intensity = _intensity;
		face_color = color;
		edge_color = ImVec4(20, 20, 20, 255);
		use_normals = false;
		ImVector<ImVec2>* v = new ImVector<ImVec2>();
		v->push_back(ImVec2(0, 0));
		v->push_back(ImVec2(2, 0));
		v->push_back(ImVec2(2, 7));
		v->push_back(ImVec2(6, 10));
		v->push_back(ImVec2(2, 8));
		v->push_back(ImVec2(0, 10));
		Primitive* forming = new Primitive(v, GetColorV4U32(edge_color), 1);
		performRotation(forming, 8, Axis::Y);
		add_point(ImVec3(1, 0, 1));
		add_point(ImVec3(-1, 0, 1));
		add_point(ImVec3(-1, 0, -1));
		size_t s = points_size();
		auto poly = Polygon({ (uint32_t)s - 3, (uint32_t)s - 2, (uint32_t)s - 1 });
		add_polygon(poly);
		translate(0, 80, 0);
	}

	inline ImVec3& getNormal() { return normal; }
	inline ImVec4& getColor() { return color; }
	inline void setColor(ImVec4& _color) { color = _color; face_color = _color; }
	inline float& getIntensity() { return intensity; }
	inline void setIntensity(float intens) { intensity = intens; }
	virtual void recalculate_normals() override
	{
		Mesh::recalculate_normals();
		normal = polygons.back().normal;
	}
};
