#include "geometry/methods/zbuffer.h"


ZBuffer::ZBuffer(size_t width, size_t height, ImVec2& _offset)
{
	resize(width, height);
	offset = _offset;
}

void ZBuffer::clear()
{
	size_t height = depthBuffer.size();
	size_t width = colorBuffer.size();
	for (size_t i = 0; i < height; ++i)
	{
		std::fill(depthBuffer[i].begin(), depthBuffer[i].end(), FLT_MAX);
		std::fill(colorBuffer[i].begin(), colorBuffer[i].end(), ImVec4(0, 0, 0, 0));
	}
}

void ZBuffer::resize(size_t width, size_t height)
{
	depthBuffer.resize(height);
	colorBuffer.resize(height);
	for (size_t i = 0; i < height; ++i)
	{
		depthBuffer[i].resize(width, FLT_MAX);
		colorBuffer[i].resize(width, ImVec4(0, 0, 0, 0));
	}
}

void ZBuffer::setOffset(ImVec2& new_offset)
{
	offset = new_offset;
}

void ZBuffer::draw(ImDrawList* draw_list, ImVec2& canvasOffset)
{
	auto c = ImU32(0);
	size_t height = colorBuffer.size();
	size_t width = colorBuffer[0].size();
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			if (colorBuffer[y][x].w == 0)
				continue;
			draw_list->AddRectFilled(ImVec2(x, y) + canvasOffset, ImVec2(x + 1, y + 1) + canvasOffset, GetColorV4U32(colorBuffer[y][x]));
		}
	}
}

void ZBuffer::fillBuffers(std::vector<Mesh*>& meshes, Eigen::Matrix4f& vp, ImVec3& cam_dir)
{
	for (size_t i = 0; i < meshes.size(); ++i) {
		auto polygons = meshes[i]->getPolygons();
		for (size_t j = 0; j < polygons.size(); ++j) {
			//if (dynamic_cast<RotationBody*>(meshes[i]) == nullptr && dynamic_cast<MeshGraph*>(meshes[i]) == nullptr)
			//if (polygons[j].normal * cam_dir >= 0)
				//continue;
			processPolygon(meshes[i], polygons[j], vp);
		}
	}
}

void ZBuffer::processPolygon(Mesh* mesh, Polygon& poly, Eigen::Matrix4f& vp)
{
	Region polygonRegion;
	ImVec2 size = ImVec2(colorBuffer[0].size(), colorBuffer.size());
	auto faceColor = mesh->getFaceColor();
	auto edgeColor = mesh->getEdgeColor();
	for (size_t i = 0; i < poly.size(); ++i)
	{
		ImVec3& origin_p0 = mesh->getPoint(poly[i]);
		ImVec3& origin_p1 = mesh->getPoint(poly[(i + 1) % poly.size()]);
		Eigen::Vector4f p0{ origin_p0.x, origin_p0.y, origin_p0.z,  1.f};
		Eigen::Vector4f p1{ origin_p1.x, origin_p1.y, origin_p1.z,  1.f };
		Eigen::Vector4f p0_3d = vp * p0;
		Eigen::Vector4f p1_3d = vp * p1;
		interpolateLine(p0_3d, p1_3d, polygonRegion, size, edgeColor);
	}
	for (auto& it : polygonRegion)
	{
		int y = it.first;
		if (it.second[0].x == it.second[1].x && it.second[0].x >= 0 && y >= 0 && it.second[0].x < size.x && y < size.y)
		{
			this->depthBuffer[y][it.second[1].x] = it.second[1].depth;
			this->colorBuffer[y][it.second[1].x] = faceColor;
			continue;
		}
		int left_x = it.second[0].x, right_x = it.second[1].x;
		float left_d = it.second[0].depth, right_d = it.second[1].depth;
		int line_length = right_x - left_x;
		float depth_diff = right_d - left_d;
		for (int x = left_x; x <= right_x; ++x)
		{
			if (x < 0 || y < 0 || x >= size.x || y >= size.y)
				continue;
			float c = (x - left_x) / (float)line_length;
			float d = left_d + c * depth_diff;
			if (d < this->depthBuffer[y][x])
			{
				this->depthBuffer[y][x] = d;
				this->colorBuffer[y][x] = faceColor;
			}
		}
	}
}

void ZBuffer::interpolateLine(Eigen::Vector4f& p0_3d, Eigen::Vector4f& p1_3d, Region& polygonRegion, ImVec2& size, ImVec4& color)
{
	auto p0_2d = ImVec2((int)(p0_3d(0) / p0_3d(3)), (int)(p0_3d(1) / p0_3d(3))) + this->offset;
	auto p1_2d = ImVec2((int)(p1_3d(0) / p1_3d(3)), (int)(p1_3d(1) / p1_3d(3))) + this->offset;
	float p0_z = p0_3d(3), p1_z = p1_3d(3);
	float alpha = 666;
	if (p1_2d.x - p0_2d.x != 0)
		alpha = abs((p1_2d.y - p0_2d.y) / (p1_2d.x - p0_2d.x));
	if (alpha > 1)
	{
		std::swap(p0_2d.x, p0_2d.y);
		std::swap(p1_2d.x, p1_2d.y);
	}
	int dx = p1_2d.x - p0_2d.x;
	if (dx < 0)
	{
		std::swap(p0_2d, p1_2d);
		std::swap(p0_z, p1_z);
		dx = abs(dx);
	}
	int dy = p1_2d.y - p0_2d.y;
	int sign_dy = dy > 0 ? 1 : -1;
	int di = sign_dy * 2 * dy - dx;
	int yi = 0;
	float line_length = distance(p0_2d, p1_2d);
	float dc = p1_z - p0_z;
	for (int xi = 0; xi <= dx; ++xi)
	{
		float c = distance(p0_2d, ImVec2(p0_2d.x + xi, p0_2d.y + yi)) / line_length;
		float new_depth = p0_z + c * dc;
		float new_x, new_y;
		if (alpha > 1)
		{
			new_x = p0_2d.y + yi;
			new_y = p0_2d.x + xi;
		}
		else
		{
			new_y = p0_2d.y + yi;
			new_x = p0_2d.x + xi;
		}
		if (new_x >= 0 && new_y >= 0 && new_x < size.x && new_y < size.y && new_depth < this->depthBuffer[new_y][new_x])
		{
			this->depthBuffer[new_y][new_x] = new_depth;
			this->colorBuffer[new_y][new_x] = color;
		}
		PointDepth newPD { new_x, new_depth };
		if (polygonRegion.find(new_y) != polygonRegion.end())
		{
			if (new_x < polygonRegion[new_y][0].x)
				polygonRegion[new_y][0] = newPD;
			if (new_x > polygonRegion[new_y][1].x)
				polygonRegion[new_y][1] = newPD;
		}
		else
		{
			polygonRegion[new_y] = std::array<PointDepth, 2> {newPD, newPD};
		}
		if (di > 0)
		{
			yi += sign_dy;
			di += 2 * dy * sign_dy - 2 * dx;
		}
		else
			di += 2 * dy * sign_dy;
	}
}
