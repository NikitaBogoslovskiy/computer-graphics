#pragma once

#include <iostream>
#include "entity.h"
#include "../../headers/shader_loader.h"

class Tetrahedron : public Entity {
	GLuint Attrib_vertex;
	GLuint Attrib_color;
	GLuint Attrib_texture;

	GLuint VBO1;
	GLuint VAO1;
	GLuint IBO1;

	void InitVBO1();
	void InitVAO1();

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

public:
	Tetrahedron();

	void Draw() override;
};