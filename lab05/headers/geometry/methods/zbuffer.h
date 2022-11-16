#pragma once

#include "geometry/primitives3d/structers.h"
#include "geometry/primitives3d/mesh.h"
#include "pch.h"
#include <array>
#include <iostream>

//auto cmp = [](std::tuple<int, float>& t1, std::tuple<int, float>& t2) {return std::get<0>(t1) < std::get<0>(t2); };
struct PointDepth
{
	int x;
	float depth;
	//bool isIncident;
	bool operator<(const PointDepth& rhs) const
	{
		return x < rhs.x;
	}
};
using ColorMatrix = std::vector<std::vector<ImVec4>>;
using DepthMatrix = std::vector<std::vector<float>>;
using Region = std::unordered_map<int, std::array<PointDepth, 2>>;

class ZBuffer
{
	ColorMatrix colorBuffer;
	DepthMatrix depthBuffer;
	ImVec2 offset;
public:
	ZBuffer() {}
	ZBuffer(size_t width, size_t height, ImVec2& offset);
	void clear();
	void resize(size_t width, size_t height);
	void setOffset(ImVec2& new_offset);
	void fillBuffers(std::vector<Mesh*>& meshes, Eigen::Matrix4f& vp, ImVec3& cam_dir);
	void draw(ImDrawList* draw_list, ImVec2& offset);
private:
	void processPolygon(Mesh* mesh, Polygon& poly, Eigen::Matrix4f& vp);
	void interpolateLine(Eigen::Vector4f& p0_3d, Eigen::Vector4f& p1_3d, Region& polygonRegion, ImVec2& size, ImVec4& color);

};
