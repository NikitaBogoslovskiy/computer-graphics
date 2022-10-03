#ifndef _FUNCS_H_
#define _FUNCS_H

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Eigen/Dense"

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline float operator*(const ImVec2& lhs, const ImVec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
inline ImVec2 operator*(const float& lhs, const ImVec2& rhs) { return ImVec2(lhs * rhs.x, lhs * rhs.y); }

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

	void rotate(const float& angle);
};

class Affine
{
public:
	static const Eigen::Matrix3f& rotate(const float& angle, const ImVec2& d = ImVec2(0.f, 0.f)) {
		float cosa = cosf(angle);
		float sina = sinf(angle);
		Eigen::Matrix3f ret { 
			{ cosa, sina, 0 }, 
			{ -sina, cosa, 0 }, 
			{ -d.x * cosa + d.y * sina + d.x, -d.x * sina - d.y * cosa + d.y, 1.f}
		};
		return ret;
	}
};

#endif
