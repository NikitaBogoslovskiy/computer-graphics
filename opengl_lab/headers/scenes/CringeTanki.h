#pragma once

#include "Scene.h"
#include "../meshes/PartedIllumiMesh.h"
#include "../meshes/IllumiMesh.h"
#include "../meshes/DynamicMesh.h"
#include "../lights/LightCasters.h"
#include "../entities/LightCube.h"

class CringeTanki : public Scene {
public:
	using mesh_type = PartedIllumiMesh;
	std::vector<mesh_type*> objects;
	mesh_type* tank;
	glm::vec3 tank_direction;
	float tank_angle;
	Camera* camera;
	glm::vec3 camera_offset;
	float camera_height;
	float camera_distance;
	float camera_pitch;
	float camera_yaw;
	float velocity = 2.f;
	float unit_angle = 50.f;
	PLS pls;
	DirLight dls;
	SpotLight sps1, sps2;
	float sps_offset_x;
	float sps_offset_y;
	float sps_height;

	enum Movement {
		FORWARD,
		BACKWARD,
		LEFT_ROTATION,
		RIGHT_ROTATION
	};

	CringeTanki();
	void LoadModels(const std::vector<inModelData>& inParams);
	void PrepareData();
	void ConfigureCamera(Camera& cam);
	void ConfigureTank();
	void ConfigureHeadlights();
	void Draw(float time_coefficient, Camera& cam) final;
	void SwitchSun() { dls.intensity = !dls.intensity; }
	void SwitchFlashlight() { sps1.intensity = !sps1.intensity; sps2.intensity = !sps2.intensity; }
	void ProcessKeyboard(Movement direction, const float& deltaTime);
};
