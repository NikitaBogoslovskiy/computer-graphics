#pragma once
#ifndef STUFF_H
#define STUFF_H

#include <exception>

#define red 1.0,0.0,0.0,1.0
#define green 0.0,1.0,0.0,1.0
#define blue 0.0,0.0,1.0,1.0
#define white 1.0,1.0,1.0,1.0

#define cyan 0.0,1.0,1.0,1.0
#define magenta 1.0,0.0,1.0,1.0
#define yellow 1.0,1.0,0.0,1.0
#define black 0.0,0.0,0.0,1.0


using uint = unsigned int;

struct p3ui32 {
	using T = uint;
	T e1, e2, e3;
	
	T& x() { return e1; }
	T& y() { return e2; }
	T& z() { return e3; }

	T& r() { return e1; }
	T& g() { return e2; }
	T& b() { return e3; }

	T& v() { return e1; }
	T& vt() { return e2; }
	T& vn() { return e3; }
};

struct coord {
	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLfloat& operator[](uint index) {
		if (index > 2) throw std::exception("aaaaaaaaa");
		return (!index ? x : (index == 1 ? y : z));
	}
};
struct vertex_texture {
	GLfloat u;
	GLfloat v;
	//GLfloat w = 1.f;
};
using normal = coord;

struct color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};

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
