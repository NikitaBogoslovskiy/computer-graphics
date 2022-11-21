#include "../headers/geometry/methods/light_buffer.h"

void LightBuffer::draw(ImDrawList* draw_list, ImVec2& canvasOffset)
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

void LightBuffer::fillBuffers(std::vector<Mesh*>& meshes, Torch* torch, Eigen::Matrix4f& vp, ImVec3& cam_pos, ImVec3& cam_dir)
{
	if (torch == nullptr)
		return;
	for (size_t i = 0; i < meshes.size(); ++i) {
		if (dynamic_cast<Torch*>(meshes[i]) != nullptr)
			continue;
		std::vector<ImVec4> colors(meshes[i]->points_size());
		calculateColors(meshes[i], torch, colors);
		auto polygons = meshes[i]->getPolygons();
		for (size_t j = 0; j < polygons.size(); ++j) {
			processPolygon(meshes[i], polygons[j], colors, vp, cam_pos);
		}
	}

	for (size_t j = 0; j < torch->polygons_size(); ++j) {
		ZBuffer::processPolygon(torch, torch->getPolygon(j), vp);
	}
}

void LightBuffer::calculateColors(Mesh* mesh, Torch* torch, std::vector<ImVec4>& colors)
{
	auto polygons = mesh->getPolygons();
	auto pointsNum = mesh->points_size();
	std::vector<std::vector<int>> incidentFaces(pointsNum);
	for (size_t j = 0; j < polygons.size(); ++j) {
		for (size_t i = 0; i < polygons[j].size(); ++i) {
			incidentFaces[polygons[j][i]].push_back(j);
		}
	}
	auto meshColor = mesh->getFaceColor();
	auto torchColor = torch->getColor();
	auto intensity = torch->getIntensity();
	auto albedo = mesh->getAlbedo();
	auto torchNormal = torch->getNormal();
	auto reverseTorchNormal = ImVec3(-torchNormal.x, -torchNormal.y, -torchNormal.z);
	ImVec4 mixedColor = { (meshColor.x / 255.f) * (torchColor.x / 255.f), (meshColor.y / 255.f) * (torchColor.y / 255.f), (meshColor.z / 255.f) * (torchColor.z / 255.f), 1.f };
	for (size_t j = 0; j < pointsNum; ++j) {
		ImVec3 finalNormal(0, 0, 0);
		auto s = incidentFaces[j].size();
		for (size_t i = 0; i < s; ++i) {
			finalNormal += mesh->getPolygon(incidentFaces[j][i]).normal;
		}
		finalNormal = normilize(finalNormal);
		auto coef = albedo * intensity * std::max(0.f, dot_product(finalNormal, reverseTorchNormal));
		ImVec4 finalColor;
		finalColor.x = mixedColor.x * coef * 255;
		finalColor.y = mixedColor.y * coef * 255;
		finalColor.z = mixedColor.z * coef * 255;
		finalColor.w = 255;
		colors[j] = finalColor;
	}
}

void LightBuffer::processPolygon(Mesh* mesh, Polygon& poly, std::vector<ImVec4>& colors, Eigen::Matrix4f& vp, ImVec3& cam_pos)
{
	LightRegion polygonRegion;
	ImVec2 size = ImVec2(colorBuffer[0].size(), colorBuffer.size());
	for (size_t i = 0; i < poly.size(); ++i)
	{
		ImVec3& origin_p0 = mesh->getPoint(poly[i]);
		ImVec3& origin_p1 = mesh->getPoint(poly[(i + 1) % poly.size()]);
		Eigen::Vector4f p0{ origin_p0.x, origin_p0.y, origin_p0.z,  1.f };
		Eigen::Vector4f p1{ origin_p1.x, origin_p1.y, origin_p1.z,  1.f };
		Eigen::Vector4f p0_3d = vp * p0;
		Eigen::Vector4f p1_3d = vp * p1;
		float p0_depth = distance(origin_p0, cam_pos);
		float p1_depth = distance(origin_p1, cam_pos);
		interpolateLine(p0_3d, p1_3d, polygonRegion, size, colors[poly[i]], colors[poly[(i + 1) % poly.size()]], p0_depth, p1_depth);
	}
	for (auto& it : polygonRegion)
	{
		int y = it.first;
		if (it.second[0].x == it.second[1].x && it.second[0].x >= 0 && y >= 0 && it.second[0].x < size.x && y < size.y)
		{
			this->depthBuffer[y][it.second[1].x] = it.second[1].depth;
			this->colorBuffer[y][it.second[1].x] = it.second[1].color;
			continue;
		}
		int left_x = it.second[0].x, right_x = it.second[1].x;
		float left_d = it.second[0].depth, right_d = it.second[1].depth;
		ImVec4 left_color = it.second[0].color, right_color = it.second[1].color;
		int line_length = right_x - left_x;
		float depth_diff = right_d - left_d;
		ImVec4 dcolor = right_color - left_color;
		for (int x = left_x; x <= right_x; ++x)
		{
			if (x < 0 || y < 0 || x >= size.x || y >= size.y)
				continue;
			float c = (x - left_x) / (float)line_length;
			float d = left_d + c * depth_diff;
			if (d < this->depthBuffer[y][x])
			{
				this->depthBuffer[y][x] = d;
				this->colorBuffer[y][x] = left_color + dcolor * c;
				this->colorBuffer[y][x].w = 255;
			}
		}
	}
}

void LightBuffer::interpolateLine(Eigen::Vector4f& p0_3d, Eigen::Vector4f& p1_3d, LightRegion& polygonRegion, ImVec2& size, ImVec4 p0_color, ImVec4 p1_color, float p0_depth, float p1_depth)
{
	auto p0_2dt = ((1.0f / p0_3d(3)) * ImVec2(-p0_3d(0), -p0_3d(1))) + this->offset;
	ImVec2 p0_2d;
	p0_2d.x = (int)p0_2dt.x;
	p0_2d.y = (int)p0_2dt.y;
	auto p1_2dt = ((1.0f / p1_3d(3)) * ImVec2(-p1_3d(0), -p1_3d(1))) + this->offset;
	ImVec2 p1_2d;
	p1_2d.x = (int)p1_2dt.x;
	p1_2d.y = (int)p1_2dt.y;
	float p0_z = p0_depth, p1_z = p1_depth;
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
		std::swap(p0_color, p1_color);
		dx = abs(dx);
	}
	int dy = p1_2d.y - p0_2d.y;
	int sign_dy = dy > 0 ? 1 : -1;
	int di = sign_dy * 2 * dy - dx;
	int yi = 0;
	float line_length = distance(p0_2d, p1_2d);
	float ddepth = p1_z - p0_z;
	ImVec4 dcolor = p1_color - p0_color;
	for (int xi = 0; xi <= dx; ++xi)
	{
		float c = distance(p0_2d, ImVec2(p0_2d.x + xi, p0_2d.y + yi)) / line_length;
		float new_depth = p0_z + c * ddepth;
		ImVec4 new_color = p0_color + dcolor * c;
		new_color.w = 255;
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
			this->colorBuffer[new_y][new_x] = new_color;
		}
		PointDepthColor newPDC{ new_x, new_depth, new_color };
		if (polygonRegion.find(new_y) != polygonRegion.end())
		{
			if (new_x < polygonRegion[new_y][0].x)
				polygonRegion[new_y][0] = newPDC;
			if (new_x > polygonRegion[new_y][1].x)
				polygonRegion[new_y][1] = newPDC;
		}
		else
		{
			polygonRegion[new_y] = std::array<PointDepthColor, 2> {newPDC, newPDC};
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