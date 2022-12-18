#include "../headers/geometry/methods/rtbuffer.h"

RTBuffer::RTBuffer(size_t width, size_t height, ImVec2& _offset)
{
	resize(width, height);
	offset = _offset;
}

void RTBuffer::clear()
{
	size_t height = colorBuffer.size();
	for (size_t i = 0; i < height; ++i) {
		std::fill(colorBuffer[i].begin(), colorBuffer[i].end(), ImVec4(0, 0, 0, 0));
	}
}

void RTBuffer::resize(size_t width, size_t height)
{
	colorBuffer.resize(height);
	for (size_t i = 0; i < height; ++i) {
		colorBuffer[i].resize(width, ImVec4(0, 0, 0, 0));
	}
}

void RTBuffer::setOffset(ImVec2& new_offset) { offset = new_offset; }

void RTBuffer::draw(ImDrawList* draw_list, ImVec2& canvasOffset)
{
	auto c = ImU32(0);
	size_t height = colorBuffer.size();
	size_t width = colorBuffer[0].size();
	int counter = 0;
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			if (colorBuffer[y][x].w == 0)
			{
				draw_list->AddRectFilled(ImVec2(x, y) + canvasOffset, ImVec2(x + 1, y + 1) + canvasOffset, GetColorV4U32(ImVec4(0, 0, 0, 255)));
				++counter;
			}
			draw_list->AddRectFilled(ImVec2(x, y) + canvasOffset, ImVec2(x + 1, y + 1) + canvasOffset, GetColorV4U32(colorBuffer[y][x]));
		}
	}
}

void RTBuffer::fillBuffers(std::vector<RTEntity*>& rt_entities, const ImVec3& direction, const ImVec3& up, const ImVec3& eye, const ImVec3& target, const ImVec2& size)
{
	int height = colorBuffer.size();
	int width = colorBuffer[0].size();
	ImVec3 normal = normilize(direction);
	ImVec3 u, v;
	u = normilize(cross_product(normal, normilize(up)));
	v = normilize(cross_product(normal, u));
	float center_x = width / 2.0f, center_y = height / 2.0f;
	float local_offset_x = center_x - offset.x, local_offset_y = center_y - offset.y;
	size_t img_x = 0, img_y = 0;
	for (float y = -center_y + local_offset_y; y < center_y + local_offset_y; ++y, ++img_y)
	{
		img_x = 0;
		for (float x = -center_x + local_offset_x; x < center_x + local_offset_x; ++x, ++img_x)
		{
			ImVec3 ray_p2 = target + u * x  + v * y;
			ImVec3 ray_dir = normilize(ray_p2 - eye);
			Ray ray{ eye, ray_dir };
			colorBuffer[img_y][img_x] = trace(ray, rt_entities);
		}
	}

}

ImVec4 mix_colors(ImVec4& color1, ImVec4& color2) {
	return ImVec4(((color1.x / 255.f) * (color2.x / 255.f)) * 255.f, ((color1.y / 255.f) * (color2.y / 255.f)) * 255.f, ((color1.z / 255.f) * (color2.z / 255.f)) * 255.f, 0.f);
}

ImVec4 sum_colors(const ImVec4& color1, const ImVec4& color2) {
	return ImVec4(std::min(255.f, color1.x + color2.x), std::min(255.f, color1.y + color2.y), std::min(255.f, color1.z + color2.z), std::min(255.f, color1.w + color2.w));
}

ImVec4 RTBuffer::trace(Ray& ray, std::vector<RTEntity*>& rt_entities)
{
	float min_mu = FLT_MAX;
	ImVec3 nearest_intersection_point;
	ImVec3 nearest_intersection_normal;
	RTEntity* nearest_entity = nullptr;
	for (auto obj : rt_entities)
	{
		float curr_mu;
		ImVec3 intersection_point, intersection_normal;
		if (obj->getNearestIntersection(ray, curr_mu, intersection_point, intersection_normal) && curr_mu < min_mu)
		{
			min_mu = curr_mu;
			nearest_entity = obj;
			nearest_intersection_point = intersection_point;
			nearest_intersection_normal = intersection_normal;
		}
	}
	if (nearest_entity == nullptr)
		return ImVec4(0, 0, 0, 255);
	RTPointLight* pl = dynamic_cast<RTPointLight*>(nearest_entity);
	if (pl != nullptr)
		//return ImVec4(0, 0, 0, 255);
		return pl->getColor();

	RTObject* nearest_object = dynamic_cast<RTObject*>(nearest_entity);
	ImVec4 result_color = { 0, 0, 0, 255 };
	if (nearest_object->getReflection() == 0 && nearest_object->getTrasparency() == 0)
	{
		for (size_t i = 0; i < rt_entities.size(); ++i)
		{
			pl = dynamic_cast<RTPointLight*>(rt_entities[i]);
			if (pl != nullptr)
			{
				bool is_illuminated = true;
				ImVec3 light_direction = normilize(pl->getCenter() - nearest_intersection_point);
				Ray local_ray{ nearest_intersection_point, light_direction };
				float mu;
				ImVec3 p, l;
				for (size_t j = 0; j < rt_entities.size(); ++j)
				{
					if (i == j) continue;
					if (rt_entities[j]->getNearestIntersection(local_ray, mu, p, l) && mu > 1)
					{
						is_illuminated = false;
						break;
					}
				}
				if (is_illuminated)
					result_color = sum_colors(result_color, pl->getIntensity() * std::max(0.0f, dot_product(nearest_intersection_normal, light_direction)) * mix_colors(nearest_object->getSurfaceColor(), pl->getColor()));
			}
		}
	}
	return result_color;

	//for (unsigned i = 0; i < spheres.size(); ++i) {
	//	if (spheres[i].emissionColor.x > 0) {
	//		// this is a light
	//		Vec3f transmission = 1;
	//		Vec3f lightDirection = spheres[i].center - phit;
	//		lightDirection.normalize();
	//		for (unsigned j = 0; j < spheres.size(); ++j) {
	//			if (i != j) {
	//				float t0, t1;
	//				if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
	//					transmission = 0;
	//					break;
	//				}
	//			}
	//		}
	//		surfaceColor += sphere->surfaceColor * transmission *
	//			std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;

}
