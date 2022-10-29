#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "pch.h"

static bool pointPositionWithEdge(const ImVec2& e1, const ImVec2& e2, const ImVec2& point)
{
	float t = e1.y;
	t = e2.y;
	float x2 = e2.x - e1.x;
	float y2 = -(e2.y - e1.y);
	float px = point.x - e1.x;
	float py = -(point.y - e1.y);
	return (py * x2 - px * y2) > 0;
}
static bool foundOnEdge(const ImVec2* start, const ImVec2* end, const float& offset, const ImVec2& point) {
	if (point.x < std::min(start->x, end->x) || point.x > std::max(start->x, end->x) || point.y < std::min(start->y, end->y) || point.y > std::max(start->y, end->y)) return false;

	auto first = ImVec2(start->x - offset, start->y);
	auto second = ImVec2(start->x + offset, start->y);
	auto third = ImVec2(end->x + offset, end->y);
	auto forth = ImVec2(end->x - offset, end->y);

	bool firstSide = pointPositionWithEdge(first, second, point);
	if (pointPositionWithEdge(second, third, point) != firstSide
		|| pointPositionWithEdge(third, forth, point) != firstSide
		|| pointPositionWithEdge(forth, first, point) != firstSide) return false;
	return true;
}


class Primitive
{
protected:
	ImVector<ImVec2>* points;
private:
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
		_connect_bounds = 0; // _connect_bounds == 2 <==> DO NOT CONNECT;  
	}
	Primitive(ImVector<ImVec2>* points, const ImU32& color, const float& thickness) {
		this->points = points;
		_color = color;
		_thickness = thickness;
		_show = true;
		_connect_bounds = 0;
	}
	Primitive(const Primitive& prim) {
		points = prim.points;
		_color = prim._color;
		_thickness = prim._thickness;
		_show = prim._show;
		_connect_bounds = prim._connect_bounds;
	}
	Primitive(Primitive&& prim) noexcept{
		points = std::move(prim.points);
		_color = std::move(prim._color);
		_thickness = std::move(prim._thickness);
		_show = std::move(prim._show);
		_connect_bounds = std::move(prim._connect_bounds);
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

	inline void clear() { points->clear(); }
	inline void pop_back() { points->pop_back(); }
	inline void pop(ImVec2* iv) { points->erase(iv); }

	virtual void draw(ImDrawList*, const ImVec2&, const ImU32& col);
	virtual void draw_polyline(ImDrawList*, const ImVec2&, const ImU32& col);
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

	inline size_t find_point(const ImVec2& sample) {
		for (int ind = 0; ind < (int)size(); ind++) {
			auto dx = this->at(ind).x - sample.x;
			auto dy = this->at(ind).y - sample.y;
			if (dx * dx + dy * dy <= std::max(9.f, thickness() * thickness())) { // just for user convenience - not having them pixelhunting in case of microscopic thickness
				return ind;
			}
		}
		return size();
	}

	inline size_t find_edge(const ImVec2& sample) {
		for (int ind = 0; ind < (int)size() - 1; ind++) {
			if (foundOnEdge(&this->at(ind), &this->at(ind + 1), std::max(this->thickness() * (float)0.5, 10.f), sample)) {
				return ind;
			}
		}
		if (_connect_bounds!=2 && foundOnEdge(&this->at(size() - 1), &this->at(0), std::max(this->thickness() * (float)0.5, 10.f), sample)) return size() - 1;
		return size();
	}

	inline int point_ind(const ImVec2* sample) {
		return this->points->index_from_ptr(sample); //sample - this->points->Data;
	}

	//rotate(const float& angle, const ImVec2& d = ImVec2(0.f, 0.f))
	void rotate(const float& angle, const ImVec2* origin);

	//scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2& d = ImVec2(0.f, 0.f))
	void scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2* origin);

	//translate(const ImVec2& d = ImVec2(0.f, 0.f))
	void translate(const ImVec2* d);

	inline Primitive& operator=(const Primitive& prim) {
		points = prim.points;
		_color = prim._color;
		_thickness = prim._thickness;
		_show = prim._show;
		_connect_bounds = prim._connect_bounds;
		return *this;
	}
};

#endif
