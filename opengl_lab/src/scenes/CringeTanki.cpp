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
		m->SetSpotLights(&sps1, &sps2);
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

float distance(float x1, float y1, float x2, float y2) { return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); }

void CringeTanki::PrepareData()
{
	srand(time(NULL));
	float min_radius = 2.f;
	float max_abs = 9.f;
	objects[0]->position = { -2, 0, -2 };
	std::vector<std::pair<float, float>> added = { std::make_pair(0, 0), std::make_pair(-2, -2) };
	std::vector<mesh_type*> dups{ objects[0], objects[1], objects[2] };
	for (size_t i = 3; i < objects.size(); i++)
	{
		int duplicates = rand() % 4 + 3;
		for (int j = 0; j < duplicates; ++j)
		{
			bool ok = false;
			float x, y;
			while (!ok)
			{
				x = -max_abs + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * max_abs)));
				y = -max_abs + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * max_abs)));
				ok = true;
				for(auto& p : added)
					if (distance(x, y, std::get<0>(p), std::get<1>(p)) < min_radius)
					{
						ok = false;
						break;
					}
			}
			added.push_back(std::make_pair(x, y));
			dups.push_back(new mesh_type(*objects[i]));
			dups.back()->position = glm::vec3(x, 0, y);
			dups.back()->angle = rand() % 360;
			dups.back()->scale = i == 3 ? glm::vec3(0.023f) : glm::vec3(0.25f);
		}
	}
	objects.clear();
	std::copy(dups.begin(), dups.end(), std::back_inserter(objects));
}

void CringeTanki::ConfigureCamera(Camera& cam)
{
	camera_height = 0.8f;
	camera_distance = 1.8f;
	camera_pitch = -10.f;
	camera_yaw = -180.f;
	camera = &cam;
	camera->Position = tank->position - tank_direction * camera_distance;
	camera->Position.y = camera->Position.g = camera->Position.t = camera_height;
	camera->Pitch = camera_pitch;
	camera->Yaw = camera_yaw;
	camera->updateCameraVectors();
	camera->needsUpdateView = true;
}

void CringeTanki::ConfigureTank()
{
	tank_direction = glm::vec3(-1.f, 0.f, 0.f);
	tank_angle = 0.f;
	sps_offset_x = 0.295f;
	sps_offset_y = 0.2f;
	sps_height = 0.2f;
}

void CringeTanki::ConfigureHeadlights()
{
	sps1.direction = tank_direction;
	auto x_direction = glm::normalize(glm::cross(tank_direction, glm::vec3(0, 1, 0)));
	sps1.position = tank->position + tank_direction * sps_offset_y + x_direction * sps_offset_x;
	sps1.position.y = sps1.position.g = sps1.position.t = sps_height;
	sps2.direction = tank_direction;
	sps2.position = tank->position + tank_direction * sps_offset_y - x_direction * sps_offset_x;
	sps2.position.y = sps2.position.g = sps2.position.t = sps_height;
}

void CringeTanki::Draw(float time_coefficient, Camera& cam)
{
	auto tank_model = glm::mat4(1.0f);
	tank_model = glm::translate(tank_model, tank->position);
	tank_model = glm::rotate(tank_model, glm::radians(tank_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	tank_model = glm::scale(tank_model, glm::vec3(0.25f));
	tank->Draw(tank_model, cam);

	auto field_model = glm::mat4(1.0f);
	field_model = glm::translate(field_model, objects[1]->position);
	field_model = glm::scale(field_model, glm::vec3(0.5f));
	objects[1]->Draw(field_model, cam);

	auto tree_model = glm::mat4(1.0f);
	tree_model = glm::translate(tree_model, objects[2]->position);
	tree_model = glm::scale(tree_model, glm::vec3(0.45f));
	objects[2]->Draw(tree_model, cam);

	for (int i = 2; i < objects.size(); ++i) {
		auto _model = glm::mat4(1.0f);
		_model = glm::translate(_model, objects[i]->position);
		_model = glm::rotate(_model, glm::radians(objects[i]->angle), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, objects[i]->scale);
		objects[i]->Draw(_model, cam);
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
		sps1.position += tank_direction * v;
		sps2.position += tank_direction * v;
		camera->Position = tank->position - tank_direction * camera_distance;
		camera->Position.y = camera->Position.g = camera->Position.t = camera_height;
	}
	if (movement == BACKWARD)
	{
		tank->position -= tank_direction * v;
		sps1.position -= tank_direction * v;
		sps2.position -= tank_direction * v;
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
		ConfigureHeadlights();
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
		ConfigureHeadlights();
	}
	camera->updateCameraVectors();
	camera->needsUpdateView = true;
}
