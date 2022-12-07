#pragma once 

#include "pch.h"
#include "structers.h"
#include "camera.h"
#include "geometry/methods/funcs.h"
#include <deque>

class Mesh : public VisualParams, public MaterialParams
{
protected:
	std::vector<ImVec3> points;
	std::vector<Polygon> polygons;
	std::vector<ImVec2> vt; // vertex texture
	ImVec4 edge_color;
	bool use_normals = true;
	CringeImage image;
	float albedo = 0.18;
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

	inline std::vector<Polygon>& getPolygons() {
		return polygons;
	}
	inline Polygon& getPolygon(size_t idx) {
		return polygons[idx];
	}
	inline ImVec4& getEdgeColor() {
		return edge_color;
	}
	inline float& getAlbedo() {
		return albedo;
	}
	inline Polygon& operator[](size_t idx) {
		return polygons[idx];
	}
	inline bool getUseNormals() { return use_normals; }
	inline void setUseNormals(bool value) { use_normals = value; }
	inline ImU32 getImagePixelU32(ImVec2 uv) { 
		return image.loaded() ? image.get_pixelU32(uv) : color;
	}
	inline ImVec4 getImagePixelV4(ImVec2 uv) {
		return image.loaded() ? image.get_pixelV4(uv) : face_color;
	}
	inline void loadImage(const char* path) { return image.load(path); }
	inline bool loadedImage() { return image.loaded(); }
	inline bool hasVT() { return !vt.empty(); }
	inline const CringeImage& img() { return image; }
	inline const ImVec2& getUV(size_t idx) { return vt[idx]; }

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
	void reverse();

private:
	void _draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir, size_t start, size_t end);
public:
	void draw(ImDrawList* draw_list, const ImVec2& offset, const Eigen::Matrix4f& vp, const ImVec3& cam_dir);
	void save(const char* filename);
	void open(const char* filename);
protected:
	void calculate_normal(Polygon& p);
	virtual void recalculate_normals();
public:
	static void save_s(const char* filename, const std::set<Mesh*>& meshes);
	static void open_s(const char* filename, std::vector<Mesh*>& meshes);

private:
	//Барицентрический тест
	bool BarTest(const Polygon& p, const ImVec3& sp, const ImVec3& direction, float&t) const;
public:
	std::pair<float, ImVec3> is_intersected_with_ray(const ImVec3& sp, const ImVec3& direction, bool use_normals = true) const;
	bool is_intersected_with_light(const ImVec3& sp, const ImVec3& direction, bool use_normals = true, float max_t = FLT_MAX) const;
};

static Mesh open(const char* filename) {
	Mesh t;
	return (t.open(filename), t);
}