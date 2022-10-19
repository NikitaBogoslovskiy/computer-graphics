#pragma once
#include <set>
#include "mesh.h"

class Polyhedron : public Mesh
{
protected:
	uint8_t polygons;
public:
	static const std::set<uint8_t> available_polyhedrons;
	Polyhedron() {}
};

// 6
class Cube : public Polyhedron {
public:
	Cube(const ImVec3& lhs, const ImVec3& rhs) {
		// square 
		ImVec3 center = (lhs + rhs) / 2;
		double R = (int)sqrt(pow(lhs.x - center.x, 2) + pow(lhs.y - center.y, 2)); //+ pow(lhs.z - center.z, 2)
		double phi = acos(abs(lhs.x - center.x) / R);
		float PI2 = PI * 2;
		for (int i = 0; i < 8; i++) {			
			int x = (int)(center.x + R * cos(phi + PI2 * i / 4));
			int y = (int)(center.y + R * sin(phi + PI2 * i / 4));
			add_point(ImVec3(x, y, 0));
		}

		//add_point(lhs);
		//add_point(lhs);
	}
};

// 4
class Tetrahedron : public Polyhedron
{
public:
	Tetrahedron(const ImVec3& lhs, const ImVec3& rhs) {
		//add_point(ImVec3(lhs.x, lhs.y, lhs.z));
	}
};

// 8
class Octahedron : public Polyhedron {};

// 12
class Dodecahedron : public Polyhedron {};

// 20
class Icosahedron : public Polyhedron {};