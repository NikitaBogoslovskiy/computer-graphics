#pragma once

#include "entity.h"
#include "../../headers/shader_loader.h"
#include "../../headers/entities/MixedCube.h"

class Cube2Tex : public MixedCube {
	GLuint Attrib_vertex;
	GLuint Attrib_color;
	GLuint Attrib_texture;

	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	GLuint texture1;

	float vertices;

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

public:

	Cube2Tex();

	void Draw(const float& time) override;
};
