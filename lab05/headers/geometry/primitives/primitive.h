#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H

#include "pch.h"

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
