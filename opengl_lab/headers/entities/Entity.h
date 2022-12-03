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

	glm::mat4 projection;

public:
	Entity() {
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	}
	float velocity = 0.01f;
	coord offset = { 0.0f, 0.0f, -3.f };

	virtual void Draw(const float& time) = 0;

	void Release();
};
