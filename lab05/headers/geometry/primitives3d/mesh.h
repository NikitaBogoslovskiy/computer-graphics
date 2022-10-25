#pragma once 

#include "pch.h"
#include "structers.h"
#include "camera.h"
#include "geometry/methods/funcs.h"
#include <deque>

class Mesh : public VisualParams
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
	inline size_t polygons_size() { return polygons.size(); }
	inline size_t points_size() { return points.size(); }

	inline ImVec3& getPoint(size_t idx) {
		return points[idx];
	}
	inline Polygon& operator[](size_t idx) {
		return polygons[idx];
	}

	void draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp);
	void save(const char* filename);
	void open(const char* filename);
};

static Mesh&& open(const char* filename) {
	Mesh t;
	return (t.open(filename), std::move(t));
}