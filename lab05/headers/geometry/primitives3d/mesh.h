#pragma once 

#include "pch.h"
#include "structers.h"
#include "geometry/funcs.h"
#include <deque>

class Mesh
{
private:
	std::vector<ImVec3> points;
	std::deque<Polygon> polygons;
public:
	Mesh() noexcept : points(), polygons() {}
	inline void add_point(const ImVec3& point) { points.push_back(point); }
	inline void add_point(ImVec3&& point) { points.push_back(std::move(point)); }
	inline void add_polygon(const Polygon& polygon) { polygons.push_back(polygon); }
	inline void add_polygon(Polygon&& polygon) { polygons.push_back(std::move(polygon)); }
	inline ImVec3 center() { return sum(points) / points.size(); }

	void draw(ImDrawList* draw_list);
};