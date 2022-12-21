#pragma once
#ifndef WORLDOFTANK_H
#define WORLDOFTANK_H

#include "Scene.h"
#include "../meshes/PartedIllumiMesh.h"
#include "../meshes/IllumiMesh.h"
#include "../meshes/DynamicMesh.h"
#include "../lights/LightCasters.h"
#include "../Commander.h"
#include "../entities/Tetrahedron.h"

class WorldOfTank : public Scene {
public:
	//using mesh_type = IllumiMesh;
	using mesh_type = PartedIllumiMesh;

private:
	glm::vec3 tank_dir = {-1.f, 0.f, 0.f};
	float tank_angle = PI;
	mesh_type* tank = new mesh_type();
	mesh_type* tree = new mesh_type();
	mesh_type* field = new mesh_type();
	std::vector<mesh_type*> objects;
	std::vector<mesh_type*> enemies;
	std::vector<float> enemies_angle;

	Tetrahedron* bullet = nullptr;
	glm::vec3 bullet_dir;

	//**lights
	PLS BulletLight;
	DirLight SunLight;
	SpotLight HeadLight;
	
	sf::Clock clock;
	Commander commander;

	float tree_rad = 0.45f;
	float tank_rad = 0.35f;
	float obj_rad = 0.15f;
	float bullet_rad = 0.05f;

public:
	WorldOfTank();
	void LoadModels(const inModelData& _tank, const inModelData& _tree, const inModelData& _field, const std::vector<inModelData>& inParams, const std::vector<inModelData>& enemies);
	void GeneratePositions();
	void Draw(float time_coefficient, Camera& cam) final;
private:
	float elapsedTime;
	float deltaTime;

	bool isCamDirty = false;
	glm::vec2 mousePos;
	glm::vec2 mouseDelta;

	struct settings {
		bool is_left = false;
		bool is_right = false;
		bool is_up = false;
		bool is_down = false;

		bool is_arrow_up = false;
		bool is_arrow_down = false;

		bool is_lshift = false;
		bool is_lctrl = false;

		bool is_cam_active = false;

		bool decrease_ratio = false;
		bool increase_ratio = false;
		bool increase_zOffset = false;
		bool decrease_zOffset = false;

		bool has_bullet = true;
	} settings;
public:
	void PollEvents(sf::Window& window);
private:
	inline void SwitchHeadLight() {
		HeadLight.intensity = !HeadLight.intensity;
	}
	void Shoot();
};

#endif // !WORLDOFTANKS_H