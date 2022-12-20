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

#define ff first.first
#define fs first.second

using uint = unsigned int;

struct p3ui32 {
	using T = uint;
	T e1 = 0, e2 = 0, e3 = 0;
	
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
struct normal : public coord {
	normal(){};
	normal(GLfloat&& _x, GLfloat&& _y, GLfloat&& _z) {
		x = std::move(_x);
		y = std::move(_y);
		z = std::move(_z);
	}
	inline GLfloat length() const {
		return sqrtf(powf(x, 2.f) + powf(y, 2.f) + powf(z, 2.f));
	}
	inline normal& normilize() {
		GLfloat len = length();
		x /= len;
		y /= len;
		z /= len;
		return *this;
	}
};

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

struct Material {
	glm::vec4 ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
	glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
	glm::vec4 specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
	glm::vec4 emission = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);
	float shininess = 67.f;
	float roughness = 0.01;
	float reflectivity = 0.8;
};

struct inModelData {
	const char* obj_file;
	const char* vShader_path;
	const char* fShader_path;
	const char* texture_path;
};

constexpr float PI = 3.14159265359f;
constexpr float DPI = 6.28318530718f;
constexpr float a30 = PI / 6;
constexpr float a60 = PI / 3;
constexpr float a90 = PI / 2;
constexpr float a120 = a60 * 2;
constexpr float a150 = a120 + a30;

static GLfloat&& randf() {
	return rand() / 32'767.f;
}

#endif // !STUFF_H
