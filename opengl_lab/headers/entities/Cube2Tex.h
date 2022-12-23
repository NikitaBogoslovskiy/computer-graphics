#pragma once

#include "entity.h"
#include "../../headers/shader_loader.h"
#include "../../headers/entities/MixedCube.h"

class Cube2Tex : public MixedCube {

	GLuint Attrib_color;
	GLuint texture1;

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

public:

	Cube2Tex();

	void Draw(const glm::mat4& model, Camera& cam) override;
};
