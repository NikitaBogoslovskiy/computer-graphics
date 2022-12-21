#include "WorldOfTank.h"
#include "../headers/pch.h"
#include "../headers/entities/Skybox.h"

static void loader(WorldOfTank::mesh_type* m, const inModelData& data) {
	m->InitShader();
	m->Load(data.obj_file);
	if (data.vShader_path && *data.vShader_path) {
		m->ChangeShaders(data.vShader_path, data.fShader_path);
	}
	m->InitTextures((char*)data.texture_path);
	m->InitVO();
}

WorldOfTank::WorldOfTank()
{
	BulletLight.intensity = 0.f;
	BulletLight.ambient = glm::vec4(0.f, 0.f, 0.f, 1.f);
	BulletLight.diffuse = glm::vec4(1.f, 1.f, 1.f, 1.0f);
	BulletLight.specular = glm::vec4(0.f, 0.f, 0.f, 1.0f);
	BulletLight.attenuation = glm::vec3(1.5f, 0.007f, 0.0014f);
	skybox = new Skybox();
}

static float&& randf_r(const float& d = 5.f) {
	return (randf() - 0.5f) * d;
}


void WorldOfTank::LoadModels(const inModelData& _tank, const inModelData& _tree, const inModelData& _field, const std::vector<inModelData>& inParams, const std::vector<inModelData>& _enemies)
{
	for (auto& imd : inParams) {
		mesh_type* m = new mesh_type();
		
		loader(m, imd);
		m->SetPLS(&BulletLight);
		m->SetDirLight(&SunLight);
		m->SetSpotLight(&HeadLight);
		
		objects.push_back(std::move(m));
	}

	for (auto& imd : _enemies) {
		mesh_type* m = new mesh_type();

		loader(m, imd);
		m->SetPLS(&BulletLight);
		m->SetDirLight(&SunLight);
		m->SetSpotLight(&HeadLight);

		enemies_angle.push_back(randf_r(DPI));

		enemies.push_back(std::move(m));
	}

	loader(tank, _tank);
	tank->SetPLS(&BulletLight);
	tank->SetDirLight(&SunLight);
	tank->SetSpotLight(&HeadLight);

	loader(tree, _tree);
	tree->SetPLS(&BulletLight);
	tree->SetDirLight(&SunLight);
	tree->SetSpotLight(&HeadLight);

	loader(field, _field);
	field->SetPLS(&BulletLight);
	field->SetDirLight(&SunLight);
	field->SetSpotLight(&HeadLight);
}

void WorldOfTank::GeneratePositions()
{
	srand(time(0));
	tree->position = { 0.f, 0.f , 0.f };
	field->position = { 0.f, 0.f , 0.f };
	float coef = 5.f;
	if (!objects.empty()) {
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->position = { randf_r(coef), 0.0, randf_r(coef) };
		
			while (true) {
				if (length(objects[i]->position, tree->position) < tree_rad + obj_rad + 0.05f) {
					objects[i]->position = { randf_r(coef), 0.0, randf_r(coef) };
					continue;
				}
				for (size_t j = 0; j < i; j++) {
					if (length(objects[i]->position, objects[j]->position) < obj_rad + obj_rad + 0.05f) {
						objects[i]->position = { randf_r(coef), 0.0, randf_r(coef) };
						continue;
					}
				}
				break;
			}

		}
	}

	if (!enemies.empty()) {
		for (size_t i = 0; i < enemies.size(); i++)
		{
			enemies[i]->position = { randf_r(coef), 0.0, randf_r(coef) };

			while (true) {
				if (length(enemies[i]->position, tree->position) < tree_rad + tank_rad + 0.05f) {
					enemies[i]->position = { randf_r(coef), 0.0, randf_r(coef) };
					continue;
				}
				for (size_t j = 0; j < i; j++) {
					if (length(enemies[i]->position, enemies[j]->position) < tank_rad + tank_rad + 0.05f) {
						enemies[i]->position = { randf_r(coef), 0.0, randf_r(coef) };
						continue;
					}
				}
				for (size_t j = 0; j < objects.size(); j++) {
					if (length(enemies[i]->position, objects[j]->position) < tank_rad + obj_rad + 0.05f) {
						enemies[i]->position = { randf_r(coef), 0.0, randf_r(coef) };
						continue;
					}
				}
				break;
			}

		}
	}

	tank->position = { randf_r(coef), 0.0, randf_r(coef) };
	while (true) {
		if (length(tank->position, tree->position) < tree_rad + tank_rad + 0.1f) {
			tank->position = { randf_r(coef), 0.0, randf_r(coef) };
			continue;
		}
		for (size_t j = 0; j < objects.size(); j++) {
			if (length(tank->position, objects[j]->position) < tank_rad + obj_rad + 0.1f) {
				tank->position = { randf_r(coef), 0.0, randf_r(coef) };
				continue;
			}
		}
		for (size_t j = 0; j < enemies.size(); j++) {
			if (length(tank->position, enemies[j]->position) < tank_rad + tank_rad + 0.1f) {
				tank->position = { randf_r(coef), 0.0, randf_r(coef) };
				continue;
			}
		}
		break;
	}
}

void mistic_func(WorldOfTank::mesh_type* o, Camera& cam) {
	auto _model = glm::mat4(1.0f);
	_model = glm::translate(_model, o->position);
	//_model = glm::rotate()_model, glm::radians(time_coefficient * 500.f), glm::vec3(0.0f, 1.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(0.25f));
	o->Draw(_model, cam);
}

void WorldOfTank::Draw(float time_coefficient, Camera& cam)
{
	float dist = 0.8f;
	commander.SetPosition(tank->position + glm::vec3(0.f, dist, 0.f) - tank_dir * dist);
	commander.SetYaw(tank_angle / PI * 180.f);

	if (HeadLight.intensity > 0.0) {
		HeadLight.position = tank->position + glm::vec3(0.f, 0.1f, 0.f) + tank_dir * 0.6f;
		HeadLight.direction = tank_dir;
	}

	for (mesh_type* o : objects) {
		mistic_func(o, commander);
	}
	
	for (size_t i = 0; i < enemies.size(); i++) {
		auto _model = glm::mat4(1.0f);
		_model = glm::translate(_model, enemies[i]->position);
		_model = glm::rotate(_model, enemies_angle[i], glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::scale(_model, glm::vec3(0.25f));
		enemies[i]->Draw(_model, commander);
	}

	auto _model = glm::mat4(1.0f);
	_model = glm::translate(_model, tank->position);
	_model = glm::rotate(_model, PI - tank_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	_model = glm::scale(_model, glm::vec3(0.25f));
	tank->Draw(_model, commander);

	mistic_func(tree, commander);
	mistic_func(field, commander);

	if (bullet) {
		
		if (length(bullet->position - glm::vec3(0.f, 0.35f, 0.f), tree->position) < bullet_rad + tree_rad) {
			delete bullet;
			bullet = nullptr;
			settings.has_bullet = true;
			BulletLight.intensity = 0.f;
		}
		else {
			if (bullet) {
				for (size_t j = 0; j < objects.size(); j++) {
					if (length(bullet->position - glm::vec3(0.f, 0.35f, 0.f), objects[j]->position) < bullet_rad + obj_rad) {
						delete bullet;
						bullet = nullptr;
						settings.has_bullet = true;
						BulletLight.intensity = 0.f;
						break;
					}
				}
			}
			if (bullet) {
				for (size_t j = 0; j < enemies.size(); j++) {
					if (length(bullet->position - glm::vec3(0.f, 0.35f, 0.f), enemies[j]->position) < bullet_rad + tank_rad) {
						delete bullet;
						bullet = nullptr;
						settings.has_bullet = true;
						BulletLight.intensity = 0.f;

						enemies.erase(enemies.begin() + j);
						enemies_angle.erase(enemies_angle.begin() + j);
						break;
					}
				}
			}
		}
		if (bullet) {
			bullet->position += bullet_dir * 0.1f;
			if (length(bullet->position - glm::vec3(0.f, 0.35f, 0.f), tree->position) > 10.f) {
				delete bullet;
				bullet = nullptr;
				settings.has_bullet = true;
				BulletLight.intensity = 0.f;
			}
			else {
				BulletLight.position = glm::vec4(bullet->position, 1.f);

				auto _model = glm::mat4(1.0f);
				_model = glm::translate(_model, bullet->position);
				_model = glm::rotate(_model, glm::radians(time_coefficient * 500.f), glm::vec3(1.0f, 1.0f, 1.0f));
				_model = glm::scale(_model, glm::vec3(0.15f));
				bullet->Draw(_model, commander);
			}
		}
	}

	if (skybox) {
		// skybox should be rendered last for optimization
		skybox->Draw(glm::mat4(1.0f), commander);
	}
}

void WorldOfTank::PollEvents(sf::Window& window)
{
	deltaTime = clock.getElapsedTime().asSeconds();
	elapsedTime += deltaTime;
	clock.restart();

	sf::Event event;

	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				settings.is_up = true;
				break;

			case sf::Keyboard::A:
				settings.is_left = true;
				break;

			case sf::Keyboard::S:
				settings.is_down = true;
				break;

			case sf::Keyboard::D:
				settings.is_right = true;
				break;

			case sf::Keyboard::C:
				window.setMouseCursorVisible(settings.is_cam_active);
				settings.is_cam_active = !settings.is_cam_active;
				window.setMouseCursorGrabbed(settings.is_cam_active);
				break;

			case sf::Keyboard::F: {
				SwitchHeadLight();
			} break;

			case sf::Keyboard::Up:
				settings.is_arrow_up = true;
				break;

			case sf::Keyboard::Down:
				settings.is_arrow_down = true;
				break;

			default:
				break;
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				settings.is_up = false;
				break;

			case sf::Keyboard::A:
				settings.is_left = false;
				break;

			case sf::Keyboard::S:
				settings.is_down = false;
				break;

			case sf::Keyboard::D:
				settings.is_right = false;
				break;

			case sf::Keyboard::Up:
				settings.is_arrow_up = false;
				break;

			case sf::Keyboard::Down:
				settings.is_arrow_down = false;
				break;

			default:
				break;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				Shoot();
				break;
			default:
				break;
			}
		}
		else if (event.type == sf::Event::MouseWheelMoved)
		{
			commander.ProcessMouseScroll(event.mouseWheel.delta);
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			if (!isCamDirty) {
				mousePos.x = event.mouseMove.x;
				mousePos.y = event.mouseMove.y;
				isCamDirty = true;
			}
			mouseDelta.x += event.mouseMove.x - mousePos.x;
			mouseDelta.x = glm::clamp(mouseDelta.x, -350.f, 350.f);
			mouseDelta.y = event.mouseMove.y - mousePos.y;

			if (settings.is_cam_active) {
				const sf::Vector2i& halfsize = { (int)window.getSize().x / 2 , (int)window.getSize().y / 2 };
				sf::Mouse::setPosition(halfsize, window);
				mousePos.x = halfsize.x;
				mousePos.y = halfsize.y;
			}
		}
		else if (event.type == sf::Event::Resized) {
			glViewport(0, 0, event.size.width, event.size.height);
		}
	}
	
	if (settings.is_arrow_up)    commander.IncVelocity();
	if (settings.is_arrow_down)  commander.DecVelocity();

	if (settings.is_left)  tank_angle -= 0.01f;
	if (settings.is_right) tank_angle += 0.01f;

	if (tank_angle > DPI) tank_angle -= DPI;
	if (tank_angle < 0.f) tank_angle += DPI;

	tank_dir = glm::vec3(glm::cos(tank_angle), 0.f, glm::sin(tank_angle));

	if (settings.is_up) tank->position += tank_dir * 0.01f;
	if (settings.is_down) tank->position -= tank_dir * 0.01f;

	if (length(tank->position, tree->position) < tank_rad + tree_rad) {
		if (settings.is_up) tank->position -= tank_dir * 0.01f;
		if (settings.is_down) tank->position += tank_dir * 0.01f;
	}
	else {
		for (size_t j = 0; j < objects.size(); j++) {
			if (length(tank->position, objects[j]->position) < tank_rad + obj_rad) {
				if (settings.is_up) tank->position -= tank_dir * 0.01f;
				if (settings.is_down) tank->position += tank_dir * 0.01f;
				break;
			}
		}
		for (size_t j = 0; j < enemies.size(); j++) {
			if (length(tank->position, enemies[j]->position) < tank_rad + tank_rad) {
				if (settings.is_up) tank->position -= tank_dir * 0.01f;
				if (settings.is_down) tank->position += tank_dir * 0.01f;
				break;
			}
		}
	}

	if (glm::abs(tank->position.x - tree->position.x) > 4.8f || glm::abs(tank->position.z - tree->position.z) > 4.8f) {
		if (settings.is_up) tank->position -= tank_dir * 0.01f;
		if (settings.is_down) tank->position += tank_dir * 0.01f;
	}

	if (settings.is_cam_active) commander.ProcessMouseMovement(mouseDelta);
}

void WorldOfTank::Shoot() {
	if (settings.has_bullet) {
		settings.has_bullet = false;
		bullet = new Tetrahedron();
		bullet->position = tank->position + tank_dir * 0.4f;
		bullet->position.y += 0.35f;
		bullet_dir = tank_dir;

		BulletLight.intensity = 1.f;
		BulletLight.position = glm::vec4(bullet->position, 1.f);
	}
}
