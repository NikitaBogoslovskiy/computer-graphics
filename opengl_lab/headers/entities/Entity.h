#pragma once
#include "gl/glew.h"
#define red 1.0,0.0,0.0,1.0
#define green 0.0,1.0,0.0,1.0
#define blue 0.0,0.0,1.0,1.0
#define white 1.0,1.0,1.0,1.0

#define cyan 0.0,1.0,1.0,1.0
#define magenta 1.0,0.0,1.0,1.0
#define yellow 1.0,1.0,0.0,1.0
#define black 0.0,0.0,0.0,1.0

struct Vertex {
	struct Coord {
		GLfloat x;
		GLfloat y;
		GLfloat z;
	} coord;
	struct Color {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	} color;
};

class Entity {

	virtual void InitShader() = 0;
	virtual void InitVO() = 0;
	virtual void ReleaseVO() = 0;

	void ReleaseShader();
	void checkOpenGLerror() {}

protected:
	GLuint Program;
	GLuint Attrib_vertex;
	
	GLuint VBO;
	GLuint VAO;

public:
	float velocity = 0.0001f;
	GLfloat offset[2] = { 0.0f, 0.0f };

	virtual void Draw(const float& time) = 0;

	void Release();
};
