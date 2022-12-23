#pragma once

#include <iostream>
#include "Entity.h"
#include "../../headers/shader_loader.h"

class LightCube : public Entity {

	GLuint Attrib_color;

	void InitVBO();

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

	glm::vec4 _color;

public:
	void SetColor(const glm::vec4& col);

	LightCube();
	void Draw(const glm::mat4& model, Camera& cam) override;
};
