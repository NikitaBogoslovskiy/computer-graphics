#pragma once

#include "rtobject.h"


class RTCube : public RTObject
{
	float edge_length;
	ImVec3 center;
	std::vector<ImVec3> vertices;
	std::vector<Polygon> polygons;
public:
	RTCube(Material* m, const ImVec3 _center, float _edge_length)
	{
		center = _center;
		edge_length = _edge_length;
		float half = edge_length / 2;
		vertices.push_back(ImVec3{ center.x - half, center.y - half, center.z - half });
		vertices.push_back(ImVec3{ center.x + half, center.y - half, center.z - half });
		vertices.push_back(ImVec3{ center.x - half, center.y - half, center.z + half });
		vertices.push_back(ImVec3{ center.x + half, center.y - half, center.z + half });
		vertices.push_back(ImVec3{ center.x - half, center.y + half, center.z - half });
		vertices.push_back(ImVec3{ center.x + half, center.y + half, center.z - half });
		vertices.push_back(ImVec3{ center.x - half, center.y + half, center.z + half });
		vertices.push_back(ImVec3{ center.x + half, center.y + half, center.z + half });

		polygons.push_back(Polygon{ 2, 0, 1 });
		polygons.push_back(Polygon{ 1, 3, 2 });
		polygons.push_back(Polygon{ 5, 4, 6 });
		polygons.push_back(Polygon{ 5, 6, 7 });
		polygons.push_back(Polygon{ 6, 2, 3 });
		polygons.push_back(Polygon{ 6, 3, 7 });
		polygons.push_back(Polygon{ 5, 1, 0 });
		polygons.push_back(Polygon{ 5, 0, 4 });
		polygons.push_back(Polygon{ 4, 0, 2 });
		polygons.push_back(Polygon{ 4, 2, 6 });
		polygons.push_back(Polygon{ 7, 3, 1 });
		polygons.push_back(Polygon{ 7, 1, 5 });

		material = m;

		recalculate_normals();
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
		for (int i = 0; i < polygons.size(); ++i)
		{
			float normalDotRay = dot_product(polygons[i].normal, ray.direction);
			if (abs(normalDotRay) < 0.0001) continue;	//ray and triangle are parallel
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
			}
		}
		if (min_mu == FLT_MAX)
			return false;
		mu = min_mu;
		intersection_point = min_point;
		intersection_normal = min_normal;
		return true;
	}
};
