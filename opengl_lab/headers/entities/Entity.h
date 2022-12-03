#pragma once
#include "gl/glew.h"
#include "../../headers/pch.h"
#include "../stuff.h"

struct Vertex {
	coord coord;
	color color;
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

public:
	Entity() {}

	float velocity = 0.1f;
	coord offset = { 0.0f, 0.0f, -3.f };

	virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) = 0;

	void Release();
};
