#pragma once 

#include "pch.h"
#include "structers.h"
#include "camera.h"
#include "geometry/methods/funcs.h"
#include <deque>

class Mesh : public VisualParams
{
protected:
	std::vector<ImVec3> points;
	std::deque<Polygon> polygons;
	ImVec4 face_color;
	ImVec4 edge_color;
public:
	Mesh();
	inline void add_point(const ImVec3& point) { points.push_back(point); }
	inline void add_point(ImVec3&& point) { points.push_back(std::move(point)); }
	virtual inline void add_polygon(const Polygon& polygon) { polygons.push_back(polygon); calculate_normal(polygons.back()); }
	virtual inline void add_polygon(Polygon&& polygon) { polygons.push_back(std::move(polygon)); calculate_normal(polygons.back()); }
	inline ImVec3 center() { return sum(points) / points.size(); }
	inline size_t polygons_size() { return polygons.size(); }
	inline size_t points_size() { return points.size(); }

	inline ImVec3& getPoint(size_t idx) {
		return points[idx];
	}

	inline std::deque<Polygon> getPolygons() {
		return polygons;
	}
	inline Polygon& getPolygon(size_t idx) {
		return polygons[idx];
	}
	inline ImVec4& getFaceColor() {
		return face_color;
	}
	inline void setFaceColor(ImVec4& _color) {
		face_color = _color;
	}
	inline ImVec4& getEdgeColor() {
		return edge_color;
	}
	inline Polygon& operator[](size_t idx) {
		return polygons[idx];
	}

	void translate(float dx, float dy, float dz);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void rotateU(ImVec3 p1, ImVec3 p2, float angle);
	void reflectX();
	void reflectY();
	void reflectZ();
	void scale(float dx, float dy, float dz);
	void updatePoints(Eigen::Matrix<float, 4, 4>& mat, bool needTranslsate = true);

private:
	void _draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir, size_t start, size_t end);
public:
	void draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir);
	void save(const char* filename);
	void open(const char* filename);
protected:
	inline void calculate_normal(Polygon& p) {
		p.normal = normilize(cross_product(points[p.indices[2]] - points[p.indices[1]], points[p.indices[0]] - points[p.indices[1]]));
	}
	virtual void recalculate_normals();
};

static Mesh open(const char* filename) {
	Mesh t;
	return (t.open(filename), t);
}