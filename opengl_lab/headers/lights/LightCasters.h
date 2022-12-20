#pragma once
#ifndef LIGHT_CASTERS_H
#define LIGHT_CASTERS_H

#include "../pch.h"

struct PointLightSource {
	glm::vec4 position = glm::vec4(0.f, 0.f, 0.f, 1.f);

	glm::vec4 ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);
	glm::vec4 diffuse = glm::vec4(0.6f, 0.6f, 0.6f, 1.6f);
	glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 attenuation = glm::vec3(0.6f, 0.00007f, 0.0000014f);
	float intensity = 0.f;
};
using PLS = PointLightSource;
using PoingLight = PointLightSource;

struct DirLight {
	glm::vec3 direction = glm::vec3(0.f, 0.0f, 1.0f);

	glm::vec4 ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0);
	glm::vec4 diffuse = glm::vec4(1.0f, 1.0, 1.0f, 1.0);
	glm::vec4 specular = glm::vec4(1.f, 1.0f, 1.0f, 1.0);
	float intensity = 1.f;
};

struct SpotLight {
	glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(15.0f));

	glm::vec4 ambient = glm::vec4(0.f);
	glm::vec4 diffuse = glm::vec4(1.f);
	glm::vec4 specular = glm::vec4(1.f);
	glm::vec3 attenuation = glm::vec3(0.6f, 0.00007f, 0.0000014f);

	float intensity = 0.f;
};
#endif // !LIGHT_CASTERS_H
