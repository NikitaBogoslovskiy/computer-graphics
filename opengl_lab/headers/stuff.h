#pragma once
#ifndef STUFF_H
#define STUFF_H

using uint = unsigned int;

struct p3ui32 {
	uint e1, e2, e3;
	
	uint& x() { return e1; }
	uint& y() { return e2; }
	uint& z() { return e3; }

	uint& r() { return e1; }
	uint& g() { return e2; }
	uint& b() { return e3; }

	uint& v() { return e1; }
	uint& vt() { return e2; }
	uint& vn() { return e3; }
};

struct coord {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct vertex_texture {
	GLfloat u;
	GLfloat v;
	//GLfloat w = 1.f;
};
using normal = coord;

struct mVertex {
	coord v;
	vertex_texture vt;
	normal vn;
};

static bool operator==(const vertex_texture& lhs, const vertex_texture& rhs) {
	return lhs.u == rhs.u && lhs.v == rhs.v;
}

static bool operator!=(const vertex_texture& lhs, const vertex_texture& rhs) {
	return !(lhs == rhs);
}

static bool operator==(const coord& lhs, const coord& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

static bool operator!=(const coord& lhs, const coord& rhs) {
	return !(lhs == rhs);
}

#endif // !STUFF_H
