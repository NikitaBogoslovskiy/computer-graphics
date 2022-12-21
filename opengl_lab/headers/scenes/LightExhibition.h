#pragma once

#include "Scene.h"
#include "../meshes/PartedIllumiMesh.h"
#include "../meshes/IllumiMesh.h"
#include "../meshes/DynamicMesh.h"
#include "../lights/LightCasters.h"
#include "../entities/LightCube.h"
#include "../game/Player.h"

class LightExhibition : public Scene {
	//using mesh_type = IllumiMesh;
	using mesh_type = PartedIllumiMesh;

	std::vector<mesh_type*> objects;
	std::vector<mesh_type*> enemies;

	//**lights
	PLS pls;		// *point light source
	LightCube lc;
	DirLight dls;

	Player* _player;

public:
	LightExhibition();
	void LoadModels(const std::vector<inModelData>& inParams,
		const std::vector<inModelData>& inEnemies,
		const std::vector<inModelData>& inMag);
	void PrepareData();
	void Draw(float time_coefficient, Camera& cam) final;
	void SetPlayer(Player* player);
	void SwitchLamp();
	void SwitchSun();
};

