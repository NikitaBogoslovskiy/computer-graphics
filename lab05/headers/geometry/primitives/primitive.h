#pragma once
#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "pch.h"

class Primitive
{
protected:
	ImVector<ImVec2>* points;
public:
	ImU32 _color;
	float _thickness;
	bool _show;
	int _connect_bounds;
public:
	Primitive(const ImU32& color, const float& thickness) {
		points = new ImVector<ImVec2>();
		_color = color;
		_thickness = thickness;
		_show = true;
		_connect_bounds = 0;
	}

	Primitive(ImVector<ImVec2>* points, const ImU32& color, const float& thickness) {
		this->points = points;
		_color = color;
		_thickness = thickness;
		_show = true;
		_connect_bounds = 0;
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

	inline void set_connect_bounds(int value) {
		_connect_bounds = value;
	}

	inline void push_back(const ImVec2& p1) {
		points->push_back(p1);
	}

	inline ImVec2& front() { return points->Data[0]; }
	inline ImVec2& back() { return points->Data[size() - 1]; }

	inline void pop_back() { points->pop_back(); }
	inline void pop(const ImVec2* it) { points->erase(it); }

	virtual void draw(ImDrawList*, const ImVec2&);
	virtual void draw_polyline(ImDrawList*, const ImVec2&);
	virtual void draw_polyline(ImDrawList*, const ImVec2&, const ImVec4& from_col, const ImVec4& col_offset, const float& from_th, const float& th_offset);

	virtual void draw_previe(ImDrawList*, const ImVec2&);

	ImVec2 center() {
		float y = 0.f, x = 0.f;
		for (size_t i = 0; i < size(); i++) {
			x += this->at(i).x;
			y += this->at(i).y;
		}
		return ImVec2(x / size(), y / size());
	}

	//rotate(const float& angle, const ImVec2& d = ImVec2(0.f, 0.f))
	void rotate(const float& angle, const ImVec2* origin);

	//scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2& d = ImVec2(0.f, 0.f))
	void scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2* origin);

	//translate(const ImVec2& d = ImVec2(0.f, 0.f))
	void translate(const ImVec2* d);
};

#endif
