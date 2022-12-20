#pragma once

#include "../../headers/pch.h"
#include "../stuff.h"
#include "../Camera.h"

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
	GLuint Attrib_vertex;

	GLuint VBO;
	GLuint IBO;

public:
	GLuint Program;
	GLuint VAO;
	Entity() {}

	float velocity = 0.1f;
	coord offset = { 0.0f, 0.0f, 0.f }; // why the fck it is here

	glm::vec3 position;
	glm::vec3 rotation;

	virtual void Draw(const glm::mat4& model, Camera& cam) = 0;

	void Release();
};
