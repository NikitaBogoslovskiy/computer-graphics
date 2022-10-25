#pragma once
#include <set>
#include "mesh.h"

class Polyhedron : public Mesh
{
protected:
	//1.6180
	float phi = (1 + sqrt(5.0))/2.f;
public:
	static const std::set<uint8_t> available_polyhedrons;
	Polyhedron() {}
};

// 4
class Tetrahedron : public Polyhedron
{
public:
	Tetrahedron(const ImVec3& center, float line = 30.f, uint8_t position = 0) {
		assert(position == 0 || position == 1);
		float hlife = 1 / sqrt(2) * line;
		if (position == 0) {
			add_point(center + ImVec3(hlife, hlife, hlife));
			add_point(center + ImVec3(hlife, -hlife, -hlife));
			add_point(center + ImVec3(-hlife, hlife, -hlife));
			add_point(center + ImVec3(-hlife, -hlife, hlife));

			add_polygon(Polygon({ 0, 1, 2 }));
			add_polygon(Polygon({ 0, 2, 3 }));
			add_polygon(Polygon({ 0, 1, 3 }));
			add_polygon(Polygon({ 1, 2, 3 }));
		}
		else {
			add_point(center + ImVec3(-hlife, -hlife, -hlife));
			add_point(center + ImVec3(hlife, hlife, -hlife));
			add_point(center + ImVec3(hlife, -hlife, hlife));
			add_point(center + ImVec3(-hlife, hlife, hlife));

			add_polygon(Polygon({ 0, 1, 2 }));
			add_polygon(Polygon({ 0, 2, 3 }));
			add_polygon(Polygon({ 0, 1, 3 }));
			add_polygon(Polygon({ 1, 2, 3 }));
		}

		updateInitPoints();
	}
};

// 6
class Cube : public Polyhedron {
public:
	Cube(const ImVec3& center, float line = 60.f) {
		float hlife = line / 2;
		add_point(center + ImVec3(hlife, hlife, hlife));
		add_point(center + ImVec3(hlife, hlife, -hlife));
		add_point(center + ImVec3(hlife, -hlife, hlife));
		add_point(center + ImVec3(hlife, -hlife, -hlife));
		add_point(center + ImVec3(-hlife, hlife, hlife));
		add_point(center + ImVec3(-hlife, hlife, -hlife));
		add_point(center + ImVec3(-hlife, -hlife, hlife));
		add_point(center + ImVec3(-hlife, -hlife, -hlife));

		updateInitPoints();

		add_polygon(Polygon({ 0, 1, 3, 2 }));
		add_polygon(Polygon({ 4, 5, 7, 6 }));
		add_polygon(Polygon({ 1, 3, 7, 5 }));
		add_polygon(Polygon({ 0, 2, 6, 4 }));
		add_polygon(Polygon({ 0, 1, 5, 4 }));
		add_polygon(Polygon({ 2, 3, 7, 6 }));
	}
};

// 8
class Octahedron : public Polyhedron {
public:
	Octahedron(const ImVec3& center, float line = 30.f) {
		float hlife = 1 / sqrt(2) * line;
		add_point(center + ImVec3(0, 0, hlife));
		add_point(center + ImVec3(hlife, 0, 0));
		add_point(center + ImVec3(0, hlife, 0));
		add_point(center + ImVec3(-hlife, 0, 0));
		add_point(center + ImVec3(0, -hlife, 0));
		add_point(center + ImVec3(0, 0, -hlife));

		updateInitPoints();

		add_polygon(Polygon({ 0, 1, 2 }));
		add_polygon(Polygon({ 0, 2, 3 }));
		add_polygon(Polygon({ 0, 3, 4 }));
		add_polygon(Polygon({ 0, 4, 1 }));
		add_polygon(Polygon({ 5, 1, 2 }));
		add_polygon(Polygon({ 5, 2, 3 }));
		add_polygon(Polygon({ 5, 3, 4 }));
		add_polygon(Polygon({ 5, 4, 1 }));
	}
};

// 20
class Icosahedron : public Polyhedron {
public:
	Icosahedron(const ImVec3& center, float line = 30.f) {
		float hlife = 1 / sqrt(2) * line;
		float phiMhl = phi * hlife;

		add_point(center + ImVec3(0, phiMhl, hlife)); // A 0
		add_point(center + ImVec3(0, -phiMhl, hlife)); // B 1
		add_point(center + ImVec3(0, phiMhl, -hlife)); // C 2
		add_point(center + ImVec3(0, -phiMhl, -hlife)); // D 3

		add_point(center + ImVec3(phiMhl, hlife, 0)); // E 4
		add_point(center + ImVec3(-phiMhl, hlife, 0)); // F 5
		add_point(center + ImVec3(phiMhl, -hlife, 0)); // G 6
		add_point(center + ImVec3(-phiMhl, -hlife, 0)); // H 7

		add_point(center + ImVec3(hlife, 0, phiMhl)); // I 8
		add_point(center + ImVec3(-hlife, 0, phiMhl)); // J 9
		add_point(center + ImVec3(hlife, 0, -phiMhl)); // K 10
		add_point(center + ImVec3(-hlife, 0, -phiMhl)); // L 11

		updateInitPoints();

		add_polygon({0, 8, 9});
		add_polygon({0, 9, 5});
		add_polygon({0, 8, 4});
		add_polygon({0, 2, 4});
		add_polygon({0, 2, 5});
		add_polygon({1, 8, 6});
		add_polygon({1, 8, 9});
		add_polygon({1, 9, 7});
		add_polygon({1, 7, 3});
		add_polygon({1, 3, 6});
		add_polygon({9, 5, 7});
		add_polygon({8, 6, 4});
		add_polygon({5, 2, 11});
		add_polygon({5, 11, 7});
		add_polygon({7, 11, 3});
		add_polygon({6, 3, 10});
		add_polygon({6, 10, 4});
		add_polygon({4, 10, 2});
		add_polygon({2, 10, 11});
		add_polygon({3, 10, 11});
	}
};

// 12
class Dodecahedron : public Polyhedron {
public:
	Dodecahedron(const ImVec3& center, float line = 30.f) {
		float hlife = 1 / sqrt(2) * line;
		float phiMhl = phi * hlife;
		float hlDphi = hlife / phi;

		add_point(center + ImVec3(0, hlDphi, phiMhl)); // A 0
		add_point(center + ImVec3(0, -hlDphi, phiMhl)); // B 1
		add_point(center + ImVec3(0, hlDphi, -phiMhl)); // C 2
		add_point(center + ImVec3(0, -hlDphi, -phiMhl)); // D 3

		add_point(center + ImVec3(hlDphi, phiMhl, 0)); // E 4
		add_point(center + ImVec3(-hlDphi, phiMhl, 0)); // F 5
		add_point(center + ImVec3(hlDphi, -phiMhl, 0)); // G 6
		add_point(center + ImVec3(-hlDphi, -phiMhl, 0)); // H 7

		add_point(center + ImVec3(phiMhl, 0, hlDphi)); // I 8
		add_point(center + ImVec3(-phiMhl, 0, hlDphi)); // J 9
		add_point(center + ImVec3(phiMhl, 0, -hlDphi)); // K 10
		add_point(center + ImVec3(-phiMhl, 0, -hlDphi)); // L 11

		add_point(center + ImVec3(hlife, hlife, hlife)); // M 12
		add_point(center + ImVec3(hlife, hlife, -hlife)); // N 13
		add_point(center + ImVec3(hlife, -hlife, hlife)); // O 14
		add_point(center + ImVec3(hlife, -hlife, -hlife)); // P 15
		add_point(center + ImVec3(-hlife, hlife, hlife)); // Q 16
		add_point(center + ImVec3(-hlife, hlife, -hlife)); // R 17
		add_point(center + ImVec3(-hlife, -hlife, hlife)); // S 18
		add_point(center + ImVec3(-hlife, -hlife, -hlife)); // T 19

		updateInitPoints();

		add_polygon({ 0, 1, 14, 8, 12 });
		add_polygon({ 0, 1, 18, 9, 16 });
		add_polygon({ 0, 12, 4, 5, 16 });
		add_polygon({ 12, 8, 10, 13, 4 });
		add_polygon({ 14, 8, 10, 15, 6 });
		add_polygon({ 18, 1, 14, 6, 7 });
		add_polygon({ 18, 7, 19, 11, 9 });
		add_polygon({ 9, 16, 5, 17, 11 });
		add_polygon({ 7, 6, 15, 3, 19 });
		add_polygon({ 4, 5, 17, 2, 13 });
		add_polygon({ 10, 13, 2, 3, 15 });
		add_polygon({ 17, 11, 19, 3, 2 });
	}
};