#pragma once

#include "../../headers/pch.h"
#include <iostream>
#include "entity.h"
#include "../../headers/shader_loader.h"

class MixedCube : public Entity {
	GLuint Attrib_vertex;
	GLuint Attrib_color;
	GLuint Attrib_texture;

	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	float vertices;
	//void InitVBO1();
	//void InitVAO1();

	//void InitShader() override;
	//void InitVO() override;
	//void ReleaseVO() override;


public:
	// god forgive me for this
	void AltMixRatio(const float& alt) {
		mixRatio += alt;
		mixRatio = std::max(0.f, mixRatio);
		mixRatio = std::min(1.f, mixRatio);
	}
	float mixRatio = 0.5f;
	float mixRatioStep = 0.0001f;

	MixedCube() {}

	//void Draw() override;
};
