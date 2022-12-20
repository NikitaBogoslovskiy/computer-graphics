#include "../headers/scenes/LightExhibition.h"
#include "../headers/pch.h"
#include "../headers/entities/Skybox.h"
#include <ctime>

LightExhibition::LightExhibition()
{
	skybox = new Skybox();
}

void LightExhibition::LoadModels(const std::vector<inModelData>& inParams)
{
	for (auto& imd : inParams) {
		mesh_type* m = new mesh_type();
		m->InitShader();
		m->Load(imd.obj_file);

		m->SetPLS(&pls);
		lc.SetColor(pls.specular);
		m->SetDirLight(&dls);
		m->SetSpotLight(&sps);

		if (imd.vShader_path && *imd.vShader_path) {
			m->ChangeShaders(imd.vShader_path, imd.fShader_path);
		}
		m->InitTextures((char*)imd.texture_path);
		m->InitVO();

		// assuming all have gone fine and we didnt get any errors :) (optimistic)
		//objects.push_back(m);
		objects.push_back(std::move(m));
	}
}

void LightExhibition::PrepareData()
{
	if (objects.size() > 1) {
		float dPhi = DPI / objects.size();
		for (size_t i = 0; i < objects.size(); i++)
		{
			// we can swap sin with cos, it doest matter
			objects[i]->position.x = 50.f * sinf(i * dPhi);
			objects[i]->position.y = 50.f * cosf(i * dPhi);
			objects[i]->position.z = 0.f;
		}
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
		//pls.diffuse = glm::vec4(abs(sin(time_coefficient)), abs(cos(time_coefficient)), abs(cos(time_coefficient)), 1.0);
		//pls.ambient = glm::vec4(pls.diffuse * glm::vec4(glm::vec3(0.5f), 1.0));
		//lc.SetColor(pls.diffuse);

		lc.Draw(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(pls.position)),
			glm::vec3(1.f)
		),
			cam);
	}
	if (sps.intensity > 0.0) {
		sps.position = cam.GetPosition();
		sps.direction = cam.GetDirection();
	}

	for (mesh_type* o : objects) {
		auto _model = glm::mat4(1.0f);
		_model = glm::translate(_model, o->position);
		_model = glm::scale(_model, glm::vec3(0.25f));
		o->Draw(_model, cam);
	}

	if (skybox) {
		// skybox should be rendered last for optimization
		skybox->Draw(glm::mat4(1.0f), cam);
	}
}

void LightExhibition::SwitchLamp() { pls.intensity = !pls.intensity; } //huehehehehuehe
void LightExhibition::SwitchSun() { dls.intensity = !dls.intensity; } //huehehehehuehe
void LightExhibition::SwitchFlashlight() { sps.intensity = !sps.intensity; } //huehehehehuehe