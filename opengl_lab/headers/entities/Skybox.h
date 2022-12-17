#pragma once

#include <vector>
#include <iostream>
#include "Entity.h"
#include "../../headers/shader_loader.h"

class Skybox : public Entity {

	void InitVBO();
	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

	GLuint cubemapTexture;
	GLuint loadCubemap(std::vector<std::string> faces);
public:
	Skybox();
	void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) override;
};