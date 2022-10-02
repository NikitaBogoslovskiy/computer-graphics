#ifndef _FUNCS_H_
#define _FUNCS_H

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

class Primitive
{
	ImVector<ImVec2>* points;
	ImU32 _color;
	float _thickness;
public:
	Primitive(const ImU32& color, const float& thickness) {
		points = new ImVector<ImVec2>();
		_color = color;
		_thickness = thickness;
	}

	Primitive(ImVector<ImVec2>* points, const ImU32& color, const float& thickness) {
		this->points = points;
		_color = color;
		_thickness = thickness;
	}

	inline size_t size() { return points->size(); }

	inline ImVec2& operator[](size_t idx) {
		return points->Data[idx];
	}
	
	inline ImU32& color() {
		return _color;
	}

	inline float& thickness() {
		return _thickness;
	}

	inline void push_back(const ImVec2& p1) {
		points->push_back(p1);
	}

	inline ImVec2& front() { return points->Data[0]; }
	inline ImVec2& back() { return points->Data[size() - 1]; }

	inline void pop_back() { points->pop_back(); }

	virtual void draw(ImDrawList*, const ImVec2&);
	virtual void draw_previe(ImDrawList*, const ImVec2&);
};

class Point : public Primitive
{
public:
	Point(const ImVec2& p, const ImU32& color, const float& thickness);

	void draw(ImDrawList*, const ImVec2&);
};

class Edge : public Primitive
{
public:
	Edge(const ImVec2& p1, const ImVec2& p2, const ImU32& color, const float& thickness);
	inline ImVec2& p1() { return front(); }
	inline ImVec2& p2() { return back(); }

};

#endif
