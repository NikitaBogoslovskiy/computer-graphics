#pragma once

#include "zbuffer.h"
#include "geometry/primitives3d/torch.h"

struct PointDepthColor
{
	int x;
	float depth;
	ImVec4 color;
	//bool isIncident;
	bool operator<(const PointDepthColor& rhs) const
	{
		return x < rhs.x;
	}
};

using LightRegion = std::unordered_map<int, std::array<PointDepthColor, 2>>;

class LightBuffer : public ZBuffer
{
public:
	LightBuffer() {}
	LightBuffer(size_t width, size_t height, ImVec2& offset): ZBuffer(width, height, offset) {}
	void fillBuffers(std::vector<Mesh*>& meshes, Torch* torch, Eigen::Matrix4f& vp, ImVec3& cam_dir);
	void draw(ImDrawList* draw_list, ImVec2& offset);
protected:
	void calculateColors(Mesh* mesh, Torch* torch, std::vector<ImVec4>& colors);
	void processPolygon(Mesh* mesh, Polygon& poly, std::vector<ImVec4>& colors, Eigen::Matrix4f& vp);
	void interpolateLine(Eigen::Vector4f& p0_3d, Eigen::Vector4f& p1_3d, LightRegion& polygonRegion, ImVec2& size, ImVec4 p0_color, ImVec4 p1_color);
};

