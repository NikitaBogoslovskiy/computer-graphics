#include "../headers/scenes/LightExhibition.h"
#include "../headers/pch.h"
#include "../headers/entities/Skybox.h"
#include <ctime>
#include <random>
#include "../headers/meshes/Bullet.h"

LightExhibition::LightExhibition()
{
	skybox = new Skybox();
}

void LightExhibition::LoadModels(const std::vector<inModelData>& inParams,
	const std::vector<inModelData>& inEnemies,
	const std::vector<inModelData>& inMag)
{
	lc.SetColor(pls.specular);
	for (auto& imd : inParams) {
		mesh_type* m = new mesh_type();
		m->InitShader();
		m->Load(imd.obj_file);

		m->SetPLS(&pls);
		m->SetDirLight(&dls);
		m->SetSpotLight(0, &(_player->hl[0])); m->SetSpotLight(1, &(_player->hl[1]));

		if (imd.vShader_path && *imd.vShader_path) {
			m->ChangeShaders(imd.vShader_path, imd.fShader_path);
		}
		m->InitTextures((char*)imd.texture_path);
		m->InitVO();

		// assuming all have gone fine and we didnt get any errors :) (optimistic)
		//objects.push_back(m);
		objects.push_back(std::move(m));
	}

	if (_player) _player->SetMesh(objects[0]);

	for (auto& imd : inEnemies) {
		mesh_type* m = new mesh_type();
		m->InitShader();
		m->Load(imd.obj_file);

		m->SetPLS(&pls);
		m->SetDirLight(&dls);
		m->SetSpotLight(0, &(_player->hl[0])); m->SetSpotLight(1, &(_player->hl[1]));

		if (imd.vShader_path && *imd.vShader_path) {
			m->ChangeShaders(imd.vShader_path, imd.fShader_path);
		}
		m->InitTextures((char*)imd.texture_path);
		m->InitVO();

		enemies.push_back(std::move(m));
	}

	for (auto& imd : inMag) {
		Bullet* m = new Bullet();
		m->InitShader();
		m->Load(imd.obj_file);

		m->SetPLS(&pls);
		m->SetDirLight(&dls);
		m->SetSpotLight(0, &(_player->hl[0])); m->SetSpotLight(1, &(_player->hl[1]));

		if (imd.vShader_path && *imd.vShader_path) {
			m->ChangeShaders(imd.vShader_path, imd.fShader_path);
		}
		m->InitTextures((char*)imd.texture_path);
		m->InitVO();

		_player->mag.push_back(std::move(m));
	}
}

void LightExhibition::PrepareData()
{
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-4.0, 4.0);

	if (objects.size() >= 2) {
		//objects[0]->position = { 0.0,0.0,0.0 }; // field
		//objects[1]->position = { 0.0,0.0,0.0 }; // christmas tree
		//float r = 2.f;
		//float dPhi = DPI / (objects.size() - 2);

		for (size_t i = 2; i < objects.size(); i++)
		{
			objects[i]->position = { dis(gen) , 0.f, dis(gen) };
			//objects[i]->position = { r * sinf(i * dPhi), 0.0, r * cosf(i * dPhi) };
		}
	}
	for (auto& enemy : enemies) {
		enemy->position = { dis(gen) , 0.f, dis(gen) };
		enemy->rotation = { 0.f, dis(gen), 0.f };
	}

	Material bulletMtl;
	bulletMtl.emission = glm::vec4{ 0.8f, 0.8f, 0.8f, 1.f };
	bulletMtl.specular = glm::vec4{ 1.f, 1.f, 1.f, 1.f };
	for (auto& bullet : _player->mag) {
		bullet->SetMaterial(bulletMtl);
	}
}
void LightExhibition::Draw(float time_coefficient, Camera& cam)
{
	if (pls.intensity > 0.0) {
		auto r = 5.f;
		pls.position.x = r * cos(time_coefficient);
		pls.position.y = 2.f; //change it to something cool
		pls.position.z = r * sin(time_coefficient);

		// cool colors.
		pls.diffuse = glm::vec4(abs(sin(time_coefficient)), abs(cos(time_coefficient)), abs(cos(time_coefficient)), 1.0);
		pls.ambient = glm::vec4(pls.diffuse * glm::vec4(glm::vec3(0.5f), 1.0));
		lc.SetColor(pls.diffuse);

		lc.Draw(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(pls.position)),
			glm::vec3(1.f)
		),
			cam);
	}

	for (mesh_type* o : objects) {
		auto _model = glm::translate(glm::mat4(1.0f), o->position);
		_model = glm::rotate(_model, glm::radians(o->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, glm::vec3(0.25f));
		o->Draw(_model, cam);
	}

	for (mesh_type* o : enemies) {
		auto _model = glm::translate(glm::mat4(1.0f), o->position);
		_model = glm::rotate(_model, glm::radians(o->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, glm::vec3(0.005f));
		o->Draw(_model, cam);
	}

	for (Bullet* o : _player->mag) {
		if (o->inMag) continue;

		auto _model = glm::translate(glm::mat4(1.0f), o->position);
		_model = glm::rotate(_model, glm::radians(o->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, glm::vec3(0.15f));
		o->Draw(_model, cam);
	}

	if (skybox) {
		// skybox should be rendered last for optimization
		skybox->Draw(glm::mat4(1.0f), cam);
	}
}

void LightExhibition::SetPlayer(Player* player)
{
	_player = player;
}

void LightExhibition::SwitchLamp() { pls.intensity = !pls.intensity; } //huehehehehuehe
void LightExhibition::SwitchSun() { dls.intensity = !dls.intensity; } //huehehehehuehe
