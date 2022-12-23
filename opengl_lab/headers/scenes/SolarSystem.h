#pragma once

#include "../headers/meshes/DynamicMesh.h"
#include "Scene.h"

const float SELF_ROTATION_ANGLE_UNIT = 40.0;
const float GENERAL_ROTATION_ANGLE_UNIT = 0.1;

class SolarSystem : public Scene
{
	DynamicMesh sun;
	DynamicMesh planet;
	size_t planets_number;
	std::vector<glm::mat4> model_matrices;

	struct PlanetParameters	{
		float movement_speed;
		float selfrotation_speed;
		float scale_factor;
		glm::vec3 rotation_axis;
	};

	struct SunParameters{
		glm::vec3 position;
		float selfrotation_speed;
		float scale_factor;
	};

	std::vector<PlanetParameters> planets_parameters;
	SunParameters sun_parameters;
	unsigned int instanced_array;

	sf::Music music;
public:
	SolarSystem(size_t _planets_number);
	void LoadModels(const char* sun_object_path, const char* sun_texture_path, const char* planet_object_path, const char* planet_texture_path);
	void PrepareData(const glm::vec3& center);
	void Draw(float time_coefficient, Camera& cam) final;
	inline sf::Music& Music() { return music; };
};
