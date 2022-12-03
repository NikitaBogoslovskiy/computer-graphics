#pragma once

#include <iostream>
#include "entity.h"
#include "../../headers/shader_loader.h"

class Tetrahedron : public Entity {

	GLuint Attrib_color;

	void InitVBO();

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

public:

	Tetrahedron();

	void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) override;
};
