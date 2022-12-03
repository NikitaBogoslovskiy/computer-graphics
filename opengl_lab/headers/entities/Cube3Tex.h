#pragma once

#include "entity.h"
#include "../../headers/shader_loader.h"
#include "../../headers/entities/MixedCube.h"

class Cube3Tex : public MixedCube {
	
	GLuint texture1;
	GLuint texture2;

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

public:

	Cube3Tex();

	void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) override;
};
