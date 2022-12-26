#pragma once

#include "rtobject.h"

struct SceneBoxParams
{
	std::vector<ImVec3> vertices;
	std::vector<Polygon> polygons;
	std::vector<Material*> materials;
};

class RTSceneBox : public RTObject
{
public:
	std::vector<ImVec3> vertices;
	std::vector<Polygon> polygons;
	std::vector<Material*> all_materials;
	Material* chosen_material;
	RTSceneBox(std::vector<Material*>& ms, float hwidth = 220, float hheight = 200, float hlength = 800) 
	{
		float offset = 250;
		vertices.push_back(ImVec3{ -hwidth, -hheight, -offset });
		vertices.push_back(ImVec3{ hwidth, -hheight, -offset });
		vertices.push_back(ImVec3{ -hwidth, -hheight, 2 * hlength - offset });
		vertices.push_back(ImVec3{ hwidth, -hheight,  2 * hlength - offset });
		vertices.push_back(ImVec3{ -hwidth, hheight, -offset });
		vertices.push_back(ImVec3{ hwidth, hheight, -offset });
		vertices.push_back(ImVec3{ -hwidth, hheight, 2 * hlength - offset});
		vertices.push_back(ImVec3{ hwidth, hheight,  2 * hlength - offset });

		polygons.push_back(Polygon{ 5, 4, 0 });
		polygons.push_back(Polygon{ 0, 1, 5 });
		polygons.push_back(Polygon{ 4, 6, 2 });
		polygons.push_back(Polygon{ 2, 0, 4 });
		polygons.push_back(Polygon{ 3, 7, 5 });
		polygons.push_back(Polygon{ 5, 1, 3 });
		polygons.push_back(Polygon{ 2, 6, 7 });
		polygons.push_back(Polygon{ 7, 3, 2 });
		polygons.push_back(Polygon{ 1, 0, 2 });
		polygons.push_back(Polygon{ 1, 2, 3 });
		polygons.push_back(Polygon{ 4, 5, 6 });
		polygons.push_back(Polygon{ 5, 7, 6 });

		for (int i = 0; i < 12; ++i)
			all_materials.push_back(new Material(*ms[i]));

		//colors.push_back(ImVec4{ 255, 255, 255, 255 });
		//colors.push_back(ImVec4{ 255, 255, 255, 255 });
		//colors.push_back(ImVec4{ 193, 18, 21, 255 });
		//colors.push_back(ImVec4{ 193, 18, 21, 255 });
		//colors.push_back(ImVec4{ 41, 130, 210, 255 });
		//colors.push_back(ImVec4{ 41, 130, 210, 255 });
		//colors.push_back(ImVec4{ 255, 255, 255, 255 });
		//colors.push_back(ImVec4{ 255, 255, 255, 255 });
		//colors.push_back(ImVec4{ 255, 255, 255, 255 });
		//colors.push_back(ImVec4{ 255, 255, 255, 255 });
		//colors.push_back(ImVec4{ 255, 255, 255, 255 });
		//colors.push_back(ImVec4{ 255, 255, 255, 255 });

		recalculate_normals();
	}

	RTSceneBox(const SceneBoxParams& params)
	{
		vertices = params.vertices;
		polygons = params.polygons;
		all_materials = params.materials;
	}

	void recalculate_normals() {
		for (int i = 0; i < polygons.size(); ++i) {
			polygons[i].normal = normilize(cross_product(vertices[polygons[i][1]] - vertices[polygons[i][0]], vertices[polygons[i][2]] - vertices[polygons[i][0]]));
		}
	}

	virtual bool getNearestIntersection(Ray& ray, float& mu, ImVec3& intersection_point, ImVec3& intersection_normal) override
	{
		float min_mu = FLT_MAX;
		ImVec3 min_point;
		ImVec3 min_normal;
		Material* min_material = nullptr;
		for (int i = 0; i < polygons.size(); ++i)
		{
			float normalDotRay = dot_product(polygons[i].normal, ray.direction);
			if (abs(normalDotRay) < 0.001) continue;	//ray and triangle are parallel
			float d = -dot_product(polygons[i].normal, vertices[polygons[i][0]]);
			float t = -(dot_product(polygons[i].normal, ray.origin) + d) / normalDotRay;
			if (t < -1) continue; //triangle is behind us
			ImVec3 p = ray.origin + t * ray.direction;
			if (dot_product(polygons[i].normal, cross_product(vertices[polygons[i][1]] - vertices[polygons[i][0]], p - vertices[polygons[i][0]])) < 0) continue;
			if (dot_product(polygons[i].normal, cross_product(vertices[polygons[i][2]] - vertices[polygons[i][1]], p - vertices[polygons[i][1]])) < 0) continue;
			if (dot_product(polygons[i].normal, cross_product(vertices[polygons[i][0]] - vertices[polygons[i][2]], p - vertices[polygons[i][2]])) < 0) continue;
			if (t < min_mu)
			{
				min_mu = t;
				min_point = p;
				min_normal = polygons[i].normal;
				min_material = all_materials[i];
			}
		}
		if (min_mu == FLT_MAX)
			return false;
		mu = min_mu;
		intersection_point = min_point;
		intersection_normal = min_normal;
		chosen_material = min_material;
		return true;
	}
};

