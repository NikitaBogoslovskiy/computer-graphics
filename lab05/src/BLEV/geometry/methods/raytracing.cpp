#include "../headers/geometry/methods/raytracing.h"
#include <thread>
Raytracing::Raytracing(){
}

Raytracing::Raytracing(const std::vector<POL*>* _pols)
{
	pols = _pols;
}

Raytracing::Raytracing(int _width, int _height, const ImVec2& _offset, const std::vector<POL*>* _pols)
{
	size = { _width, _height };
	rays.resize(_width * _height);
	offset = _offset;
	pols = _pols;

	bounds = std::vector<std::pair<int, int>>(threadnum);
	int h10 = size.height / threadnum;
	for (size_t i = 0; i < threadnum - 1; i++)
	{
		bounds[i] = { i * h10, (i + 1) * h10 };
	}
	bounds[threadnum - 1] = { (threadnum - 1) * h10, size.height };
}

void Raytracing::clear()
{
	to_clear = true;
}

void Raytracing::resize(int _width, int _height)
{
	size = { _width, _height };
	rays.resize(_width * _height);

	bounds = std::vector<std::pair<int, int>>(threadnum);
	int h10 = size.height / threadnum;
	for (size_t i = 0; i < threadnum - 1; i++)
	{
		bounds[i] = { i * h10, (i + 1) * h10 };
	}
	bounds[threadnum - 1] = { (threadnum - 1) * h10, size.height };
}

void Raytracing::setOffset(const ImVec2& new_offset)
{
	offset = new_offset;
}

void Raytracing::draw(ImDrawList* draw_list, const ImVec2& canvasOffset)
{
	//
	draw_list->AddRectFilled(canvasOffset, { canvasOffset.x + size.width, canvasOffset.y + size.height }, 0xff000000);
	
	int i = 0;
	for (int y = 0; y < size.height; ++y) {
		for (int x = 0; x < size.width; ++x, ++i) {
			if (rays[i].x == 0.f && rays[i].y == 0.f && rays[i].z == 0.f) continue;
			draw_list->AddRectFilled(ImVec2(x, y) + canvasOffset, ImVec2(x + 1, y + 1) + canvasOffset, GetColorV3U32(rays[i]));
		}
	}
}

void Raytracing::render(objects objs, Camera& cam)
{
	float d = 400;
	auto t = normilize(cam.direction());
	auto b = normilize(cross_product(t, cam.up()));
	auto v = cross_product(t, b);
	float gx = size.width / 2.f;
	float gy = size.height / 2.f;
	ImVec3 qx = 2.f * gx / (size.width - 1) * b;
	ImVec3 qy = 2.f * gy / (size.height - 1) * v;
	ImVec3 p1m = cam.direction() - gx * b - gy * v; // cam.direction() = d * t
	
	std::vector<std::thread> ts;
	for (auto& b : bounds)
	{
		ts.emplace_back([&]() {
				int i = b.first * size.width;
				for (int y = b.first; y < b.second; ++y) {
					for (int x = 0; x < size.width; ++x, ++i) {
						ImVec3&& pij = p1m + qy * y + qx * x;
						ImVec3&& r = normilize(pij);
						rays[i] =  
						renderUnit(cam.eye() + offset, r, objs, false, 4);
					}
				}
			});
	}
	for (auto& t : ts) {
		t.join();
	}
}

ImVec3 Raytracing::renderUnit(const ImVec3& sp, const ImVec3& rayDir, objects objs, bool insideMesh, int8_t last_bounces) {
	last_bounces--;
	ImVec3 Iambient, Idiffuse, Ispecular;
	ImVec3 Ilocal = { 0.f , 0.f ,0.f }, Ireflection = { 0.f , 0.f ,0.f }, Irefraction = { 0.f , 0.f ,0.f };
	ImVec3 I = { 0.f , 0.f ,0.f };
	std::pair<float, ImVec3> minDistToPolygon = { FLT_MAX , {} };
	const MaterialParams* material = 0;

	bool ch = false;
	for (const Mesh* m : objs.meshes)
	{
		std::pair<float, ImVec3> curDist;
		if ((curDist = m->is_intersected_with_ray(sp, rayDir, !insideMesh)).first < minDistToPolygon.first) {
			ch = true;
			minDistToPolygon = curDist;
			material = m;
		}
	}
	for (const Sphere * sph : objs.spheres)
	{
		std::pair<float, ImVec3> curDist;
		if ((curDist = sph->is_intersected_with_ray(sp, rayDir, !insideMesh)).first < minDistToPolygon.first) {
			ch = true;
			minDistToPolygon = curDist;
			material = sph;
		}
	}

	//searching lights

	for (const POL* pol : *pols) {
		if (pol->length_from(sp, rayDir) < minDistToPolygon.first) {
			return ImVec3((pol->i_a[0] + pol->i_d[0] + pol->i_s[0]) / 3.f * 255.f,
				(pol->i_a[1] + pol->i_d[1] + pol->i_s[1]) / 3.f * 255.f,
				(pol->i_a[2] + pol->i_d[2] + pol->i_s[2]) / 3.f * 255.f);
		}
	}

	if (!ch) return { 0.f, 0.f, 0.f };

	ImVec3 N = insideMesh ? -minDistToPolygon.second : minDistToPolygon.second;
	ImVec3 point = sp + rayDir * minDistToPolygon.first;
	const ImVec4& clr = material->getFaceColor();

	//if (!insideMesh) 
	{
		for (const POL* l : *pols) {
			auto L = l->pos - point;
			float Ldist = length(L);
			L = normilize(L);
			auto dotLN = dot_product(L, minDistToPolygon.second);
			if (dotLN <= 0) continue;
			bool cont = false;
			for (const Mesh* m : objs.meshes)
			{
				if (m->is_intersected_with_light(point + eps * L, L, !insideMesh, Ldist)) {
					cont = true;
					break;
				}
			}
			if (cont) continue;
			for (const Sphere* sph : objs.spheres)
			{
				if (sph->is_intersected_with_light(point + eps * L, L, !insideMesh, Ldist)) {
					cont = true;
					break;
				}
			}
			if (cont) continue;

			// ambient
			Iambient = by_element_product(material->ambient, l->i_a);

			Idiffuse = dotLN * by_element_product(material->diffuse, l->i_d);
	
			auto R = 2 * dotLN * minDistToPolygon.second - L;
			Ispecular = powf(dot_product(R, -rayDir), material->shine) * by_element_product(material->specular, l->i_s);
		
			Ilocal += l->att(length(l->pos - point)) * (by_element_product(Iambient + Idiffuse, toVec3(clr)) + by_element_product(Ispecular, { 255.f, 255.f, 255.f }));
		}
	}

	if (last_bounces > 0) {
		if (material->reflection != ImVec3(0.f, 0.f, 0.f)) {
			ImVec3&& reflRay = rayDir - 2 * N * (N * rayDir);
			Ireflection = renderUnit(point + eps * reflRay, reflRay, objs, insideMesh, last_bounces);
		}

		if (material->refraction != ImVec3(0.f, 0.f, 0.f)) {
			float curEta, nextEta;
			if (insideMesh) {
				curEta = material->eta;
				nextEta = 1.f;
			}
			else {
				curEta = 1.f;
				nextEta = material->eta;
			}
			float e1de2 = curEta / material->eta;
			float dotNrD = dot_product(N, rayDir);
			float discr = 1.f - powf(e1de2, 2.f) * (1.f - powf(dotNrD, 2.f));
			if (discr > 0.0f) {

				//ImVec3&& refrRay = normilize(e1de2 * rayDir - dot_product(sqrtf(discr) + (e1de2 * dotNrD), N));
				ImVec3&& refrRay = normilize(e1de2 * (rayDir - (N * dotNrD)) - (N * sqrtf(discr)));
				Irefraction = renderUnit(point + eps * refrRay, refrRay, objs, !insideMesh, last_bounces + 1);
			}
			//printf("%f %f %f\n", Irefraction.x, Irefraction.y, Irefraction.z);
		}
	}
	ImVec3 globalL = toVec3(clr) * 0.1f;
	I = Ilocal + by_element_product(material->reflection, Ireflection) + by_element_product(material->refraction, Irefraction) + globalL;

	clamp(I, 0.f, 255.f);

	return I;
}