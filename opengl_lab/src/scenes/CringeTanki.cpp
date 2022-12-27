#include "../headers/scenes/CringeTanki.h"
#include "../headers/pch.h"
#include "../headers/entities/Skybox.h"
#include <ctime>

CringeTanki::CringeTanki()
{
	skybox = new Skybox();
}

void CringeTanki::LoadModels(const std::vector<inModelData>& inParams)
{
	bool needTank = true;
	for (auto& imd : inParams) {
		mesh_type* m = new mesh_type();
		m->InitShader();
		m->Load(imd.obj_file);
		m->SetPLS(&pls);
		m->SetDirLight(&dls);
		m->SetSpotLight(&sps);
		if (imd.vShader_path && *imd.vShader_path) {
			m->ChangeShaders(imd.vShader_path, imd.fShader_path);
		}
		m->InitTextures((char*)imd.texture_path);
		m->InitVO();
		objects.push_back(std::move(m));
		if (needTank)
		{
			tank = objects[0];
			needTank = false;
		}
	}
}

void CringeTanki::PrepareData()
{
	if (objects.size() >= 2) {
		//objects[0]->position = { 0.0,0.0,0.0 }; // field
		//objects[1]->position = { 0.0,0.0,0.0 }; // christmas tree
		float r = 2.f;
		float dPhi = DPI / (objects.size() - 2);
		for (size_t i = 2; i < objects.size(); i++)
		{
			objects[i]->position = { r * sinf(i * dPhi), 0.0, r * cosf(i * dPhi) };
		}
	}
	tank_direction = glm::vec3(-1.f, 0.f, 0.f);
	tank_angle = 0.f;
}

void CringeTanki::ConfigureCamera(Camera& cam)
{
	//camera_offset = { 1.8f, 1.f, -0.01f };
	camera_height = 0.8f;
	camera_distance = 1.8f;
	camera_pitch = -15.f;
	camera_yaw = -180.f;
	camera = &cam;
	camera->Position = tank->position - tank_direction * camera_distance;
	camera->Position.y = camera->Position.g = camera->Position.t = camera_height;
	camera->Pitch = camera_pitch;
	camera->Yaw = camera_yaw;
	camera->updateCameraVectors();
	camera->needsUpdateView = true;
}

void CringeTanki::Draw(float time_coefficient, Camera& cam)
{
	if (sps.intensity > 0.0) {
		sps.position = cam.GetPosition();
		sps.direction = cam.GetDirection();
	}
	for (mesh_type* o : objects) {
		auto _model = glm::mat4(1.0f);
		_model = glm::translate(_model, o->position);
		if (o == tank)
			_model = glm::rotate(_model, glm::radians(tank_angle), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, glm::vec3(0.25f));
		o->Draw(_model, cam);
	}

	if (skybox) {
		skybox->Draw(glm::mat4(1.0f), cam);
	}
}

void CringeTanki::ProcessKeyboard(Movement movement, const float& deltaTime)
{
	float v = velocity * deltaTime;
	float a = unit_angle * deltaTime;
	if (movement == FORWARD)
	{
		tank->position += tank_direction * v;
		camera->Position = tank->position - tank_direction * camera_distance;
		camera->Position.y = camera->Position.g = camera->Position.t = camera_height;
	}
	if (movement == BACKWARD)
	{
		tank->position -= tank_direction * v;
		camera->Position = tank->position - tank_direction * camera_distance;
		camera->Position.y = camera->Position.g = camera->Position.t = camera_height;
	}
	if (movement == LEFT_ROTATION)
	{
		tank_angle += a;
		auto model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(a), glm::vec3(0.0f, 1.0f, 0.0f));
		tank_direction = model * glm::vec4(tank_direction.x, tank_direction.y, tank_direction.z, 1.0f);
		camera->Position = tank->position - tank_direction * camera_distance;
		camera->Position.y = camera->Position.g = camera->Position.t = camera_height;
		camera->Yaw -= a;
	}
	if (movement == RIGHT_ROTATION)
	{
		tank_angle -= a;
		auto model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-a), glm::vec3(0.0f, 1.0f, 0.0f));
		tank_direction = model * glm::vec4(tank_direction.x, tank_direction.y, tank_direction.z, 1.0f);
		camera->Position = tank->position - tank_direction * camera_distance;
		camera->Position.y = camera->Position.g = camera->Position.t = camera_height;
		camera->Yaw += a;
	}
	//if (movement == BACKWARD)
	//	Position -= Front * velocity;
	//if (movement == LEFT_ROTATION)
	//	Position -= Right * velocity;
	//if (movement == RIGHT_ROTATION)
	//	Position += Right * velocity;
	camera->updateCameraVectors();
	camera->needsUpdateView = true;
}
