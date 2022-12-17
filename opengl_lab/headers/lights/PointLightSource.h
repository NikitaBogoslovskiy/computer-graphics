#pragma once
#ifndef POINT_LIGHT_SOURCE_H
#define POINT_LIGHT_SOURCE_H

#include "../pch.h"

struct PointLightSource {
	glm::vec4 position = glm::vec4(0.f, 0.f, 0.f, 1.f);
	glm::vec4 ambient = glm::vec4(1.f);
	glm::vec4 diffuse = glm::vec4(1.f);
	glm::vec4 specular = glm::vec4(1.f);
	glm::vec3 attenuation = glm::vec3(0.6f, 0.00007f, 0.0000014f);
};
using PLS = PointLightSource;
using PoingLight = PointLightSource;

#endif // !POINT_LIGHT_SOURCE_H
