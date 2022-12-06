#pragma once

//#include "gl/glew.h"
//#include "../stuff.h"
//#include "../../headers/pch.h"
#include "../headers/meshes/DynamicMesh.h"


const float SELF_ROTATION_ANGLE_UNIT = 40.0;
const float GENERAL_ROTATION_ANGLE_UNIT = 0.1;

struct PlanetParameters
{
	float movement_speed;
	float selfrotation_speed;
	float scale_factor;
};

struct SunParameters
{
	glm::vec3 position;
	float selfrotation_speed;
	float scale_factor;
};

class SolarSystem
{
	DynamicMesh* sun;
	DynamicMesh* planet;
	size_t planets_number;
	glm::mat4* model_matrices;
	std::vector<PlanetParameters*> planets_parameters;
	SunParameters* sun_parameters;
	unsigned int instanced_array;
public:
	SolarSystem(size_t _planets_number);
	void LoadModels(const char* sun_object_path, const char* sun_texture_path, const char* planet_object_path, const char* planet_texture_path);
	void PrepareData(const glm::vec3& center);
	void Draw(float time_coefficient, const glm::mat4& view, const glm::mat4& projection);
};
