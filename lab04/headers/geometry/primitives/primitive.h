#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H

#include "../../../src/imgui/imgui.h"
#include "../../../src/imgui/imgui_impl_glfw.h"
#include "../../../src/imgui/imgui_impl_opengl3.h"
#include "../../../src/Eigen/Dense"

class Primitive
{
	ImVector<ImVec2>* points;
	ImU32 _color;
	float _thickness;
	bool _show;
public:
	Primitive(const ImU32& color, const float& thickness) {
		points = new ImVector<ImVec2>();
		_color = color;
		_thickness = thickness;
		_show = true;
	}

	Primitive(ImVector<ImVec2>* points, const ImU32& color, const float& thickness) {
		this->points = points;
		_color = color;
		_thickness = thickness;
		_show = true;
	}

	inline size_t size() const { return points->size(); }

	inline ImVec2& operator[](size_t idx) {
		return points->Data[idx];
	}

	inline ImVec2& at(size_t idx) {
		return points->Data[idx];
	}

	inline ImU32& color() {
		return _color;
	}

	inline float& thickness() {
		return _thickness;
	}

	inline bool& show() {
		return _show;
	}

	inline void push_back(const ImVec2& p1) {
		points->push_back(p1);
	}

	inline ImVec2& front() { return points->Data[0]; }
	inline ImVec2& back() { return points->Data[size() - 1]; }

	inline void pop_back() { points->pop_back(); }

	virtual void draw(ImDrawList*, const ImVec2&);
	virtual void draw_previe(ImDrawList*, const ImVec2&);

	void rotate(const float& angle);
};

#endif