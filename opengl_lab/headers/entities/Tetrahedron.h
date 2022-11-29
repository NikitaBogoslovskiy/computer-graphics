#pragma once

#include <iostream>
#include "entity.h"
#include "../../headers/shader_loader.h"

class Tetrahedron : public Entity {

	GLuint Attrib_color;
	GLuint IBO;

	void InitVBO();

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

public:

	Tetrahedron();

	void Draw(const float& time) override;
};
