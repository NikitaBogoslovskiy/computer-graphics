#pragma once

#include "rtobject.h"


class RTCube : public RTObject
{
	float edge_length;
	ImVec3 center;
	std::vector<ImVec3> vertices;
	std::vector<Polygon> polygons;
public:
	RTCube(const ImVec3 _center, float _edge_length)
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

		recalculate_normals();
	}

	void recalculate_normals() {
		for (int i = 0; i < polygons.size(); ++i) {
			polygons[i].normal = normilize(cross_product(vertices[polygons[i][2]] - vertices[polygons[i][1]], vertices[polygons[i][0]] - vertices[polygons[i][1]]));
		}
	}

	virtual bool getNearestIntersection(Ray& ray, float& mu, ImVec3& intersection_point, ImVec3& intersection_normal) override
	{
		float min_mu = FLT_MAX;
		ImVec3 min_point;
		ImVec3 min_normal;
		for (int i = 0; i < polygons.size(); ++i)
		{
			ImVec3 center = { 0, 0, 0 };
			for (int j = 0; j < polygons[i].size(); ++j)
				center += vertices[polygons[i][j]];
			center /= polygons[i].size();
			float curr_mu = -1 * ((ray.origin - center) * polygons[i].normal) / (ray.direction * polygons[i].normal);
			if (curr_mu < 0) continue;
			ImVec3 q = ray.origin + curr_mu * ray.direction; //intersection point
			ImVec3 a = vertices[polygons[i][2]] - vertices[polygons[i][1]];
			ImVec3 b = vertices[polygons[i][0]] - vertices[polygons[i][1]];
			float alpha = ((b * b) * (q * a) - (a * b) * (q * b)) / ((a * a) * (b * b) - pow((a * b), 2));
			float beta = ((q * b) - alpha * (a * b)) / (b * b);
			if (alpha > 1 || alpha < 0 || beta > 1 || beta < 0 || (alpha + beta) > 1)
				continue;
			if (curr_mu < min_mu)
			{
				min_mu = curr_mu;
				min_point = q;
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
