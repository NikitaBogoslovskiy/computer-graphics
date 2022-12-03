#pragma once
#include "gl/glew.h"
#include "../../headers/pch.h"
#define red 1.0,0.0,0.0,1.0
#define green 0.0,1.0,0.0,1.0
#define blue 0.0,0.0,1.0,1.0
#define white 1.0,1.0,1.0,1.0

#define cyan 0.0,1.0,1.0,1.0
#define magenta 1.0,0.0,1.0,1.0
#define yellow 1.0,1.0,0.0,1.0
#define black 0.0,0.0,0.0,1.0

struct Coord {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct Color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};

struct Vertex {
	Coord coord;
	Color color;
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
	GLuint IBO;

	glm::mat4 projection;

public:
	Entity() {
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	}
	float velocity = 0.0004f;
	GLfloat offset[2] = { 0.0f, 0.0f };
	float zOffset = -3.f;

	virtual void Draw(const float& time) = 0;

	void Release();
};
