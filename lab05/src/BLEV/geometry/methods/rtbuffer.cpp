#include "../headers/geometry/methods/rtbuffer.h"
#include <iostream>

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
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			if (colorBuffer[y][x].w == 0)
				draw_list->AddRectFilled(ImVec2(x, y) + canvasOffset, ImVec2(x + 1, y + 1) + canvasOffset, GetColorV4U32(ImVec4(0, 0, 0, 255)));
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
			colorBuffer[img_y][img_x] = trace(ray, rt_entities, 1, FLT_MAX, 0) * 255.f;
		}
	}

}

ImVec4 mix_colors(const ImVec4& color1, const ImVec4& color2) {
	return ImVec4(color1.x * color2.x, color1.y * color2.y, color1.z * color2.z, color1.w * color2.w);
}

ImVec4 sum_colors(const ImVec4& color1, const ImVec4& color2) {
	return ImVec4(std::min(1.f, color1.x + color2.x), std::min(1.f, color1.y + color2.y), std::min(1.f, color1.z + color2.z), std::min(1.f, color1.w + color2.w));
}

ImVec4 RTBuffer::computeLocalIllumination(std::vector<RTEntity*>& rt_entities, RTObject* object, Ray& ray, ImVec3& point, ImVec3& normal)
{
	ImVec4 local_color = { 0, 0, 0, 1 };
	for (size_t i = 0; i < rt_entities.size(); ++i)
	{
		auto pl = dynamic_cast<RTPointLight*>(rt_entities[i]);
		auto al = dynamic_cast<RTAmbientLight*>(rt_entities[i]);
		if (al != nullptr)
		{
			local_color = sum_colors(local_color, al->intensity * mix_colors(al->color, object->material->ambient));
		}
		else if (pl != nullptr)
		{
			bool is_illuminated = true;
			ImVec3 light_vector = pl->center - point;
			pl->computeAttenuation(length(light_vector));
			ImVec3 light_direction = normilize(light_vector);
			ImVec3 specular_dir = normilize(2 * normal * dot_product(normal, light_direction) - light_direction);
			Ray local_ray{ point, light_direction };
			float local_max_mu;
			float local_min_mu = 1;
			ImVec3 max_inter_p, max_normal;
			pl->getNearestIntersection(local_ray, local_max_mu, max_inter_p, max_normal);
			float mu;
			ImVec3 p, l;
			float transmission = 1.0;
			for (size_t j = 0; j < rt_entities.size(); ++j)
			{
				if (i == j) continue;
				if (rt_entities[j]->getNearestIntersection(local_ray, mu, p, l) && mu < local_max_mu && mu > local_min_mu)
				{
					auto obj = dynamic_cast<RTObject*>(rt_entities[j]);
					if (obj != nullptr && obj->material->refraction != 0)
						transmission *= obj->material->refraction;
					else
					{
						is_illuminated = false;
						break;
					}
				}
			}
			if (is_illuminated)
			{
				float diffuse_coefficient = transmission * (1 - pl->attenuation) * pl->intensity * std::max(0.0f, dot_product(normal, light_direction));
				float shininess = object->material->shininess;
				float specular_coefficient = shininess > 0 ? (1 - pl->attenuation) * pl->intensity * pow(std::max(0.0f, dot_product(specular_dir, -1 * ray.direction)), shininess) : 0;
				local_color = sum_colors(local_color, sum_colors(diffuse_coefficient * mix_colors(object->material->diffuse, pl->color), specular_coefficient * mix_colors(object->material->specular, pl->color)));
				local_color.w = 1.0f;
			}
		}
	}
	return local_color;
}

ImVec4 RTBuffer::computeReflection(std::vector<RTEntity*>& rt_entities, int depth, Ray& ray, ImVec3 point, ImVec3 normal)
{
	ImVec3 refldir =  2 * normal * dot_product(normal, -1 * ray.direction) + ray.direction;
	Ray reflection_ray{ point + 0.5 * refldir, refldir };
	return trace(reflection_ray, rt_entities, 1, FLT_MAX, depth);
}

ImVec4 RTBuffer::computeRefraction(std::vector<RTEntity*>& rt_entities, int depth, Ray& ray, RTObject* object, ImVec3 point, ImVec3 normal, float ior)
{
	float eta = 1 / ior;
	float rayDotNormal = -dot_product(ray.direction, normal);
	if (rayDotNormal < 0)
	{
		normal = normal * (-1);
	}
	float k = 1 - eta * eta * (1 - rayDotNormal * rayDotNormal);
	float cos_output = sqrt(k);
	ImVec3 refrdir1 = normilize(eta * ray.direction + (eta * rayDotNormal - cos_output) * normal);
	Ray refraction_ray1{ point + 0.5 * refrdir1, refrdir1 };
	float mu;
	ImVec3 intersection_point2, intersection_normal2;
	Ray final_ray, refraction_ray2;
	if (object->getNearestIntersection(refraction_ray1, mu, intersection_point2, intersection_normal2) && mu > 1)
	{
		eta = ior;
		rayDotNormal = -dot_product(refraction_ray1.direction, intersection_normal2);
		if (rayDotNormal < 0)
		{
			intersection_normal2 = intersection_normal2 * (-1);
		}
		k = 1 - eta * eta * (1 - rayDotNormal * rayDotNormal);
		cos_output = sqrt(k);
		ImVec3 refrdir2 = normilize(eta * refraction_ray1.direction + (eta * rayDotNormal - cos_output) * intersection_normal2);
		refraction_ray2 = { intersection_point2 + 0.5 * refrdir2, refrdir2 };
		final_ray = refraction_ray2;
	}
	else
		final_ray = refraction_ray1;
	return trace(final_ray, rt_entities, 1, FLT_MAX, depth);
}

bool RTBuffer::findClosest(Ray& ray, std::vector<RTEntity*>& rt_entities, float min_mu, float max_mu, ImVec3& point, ImVec3& normal, RTObject*& object, ImVec4& output_color)
{
	float closest_mu = FLT_MAX;
	RTEntity* entity = nullptr;
	for (auto obj : rt_entities)
	{
		float curr_mu;
		ImVec3 intersection_point, intersection_normal;
		if (obj->getNearestIntersection(ray, curr_mu, intersection_point, intersection_normal) && curr_mu > min_mu && curr_mu < max_mu && curr_mu < closest_mu)
		{
			closest_mu = curr_mu;
			entity = obj;
			point = intersection_point;
			normal = intersection_normal;
		}
	}
	if (entity == nullptr)
	{
		output_color = ImVec4(0, 0, 0, 1);
		return false;
	}
	RTPointLight* pl = dynamic_cast<RTPointLight*>(entity);
	if (pl != nullptr)
	{
		output_color = pl->color;
		return false;
	}
	object = dynamic_cast<RTObject*>(entity);
	RTSceneBox* box = dynamic_cast<RTSceneBox*>(object);
	if (box != nullptr)
		object->material = box->chosen_material;
	return true;
}

ImVec4 RTBuffer::trace(Ray& ray, std::vector<RTEntity*>& rt_entities, float min_mu, float max_mu, int depth)
{
	ImVec3 point, normal;
	RTObject* object = nullptr;
	ImVec4 output_color;
	if (!findClosest(ray, rt_entities, min_mu, max_mu, point, normal, object, output_color))
		return output_color;
	ImVec4 local_color = computeLocalIllumination(rt_entities, object, ray, point, normal);
	if (object->material->reflection == 0 && object->material->refraction == 0 || depth == MAX_DEPTH)
		return local_color;
	ImVec4 reflected_color, refracted_color;
	reflected_color.w = 1.0;
	refracted_color.w = 1.0;
	if (object->material->reflection > 0)
		reflected_color = computeReflection(rt_entities, depth + 1, ray, point, normal);
	if (object->material->refraction > 0)
		refracted_color = computeRefraction(rt_entities, depth + 1, ray, object, point, normal, object->material->ior);
	ImVec4 result_color = sum_colors(local_color, sum_colors(object->material->refraction * refracted_color, object->material->reflection * reflected_color));
	return result_color;
}
