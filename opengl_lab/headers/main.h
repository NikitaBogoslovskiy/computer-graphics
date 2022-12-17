#ifndef __MAIN_H__
#define __MAIN_H__

#include "gl/glew.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "shader_loader.h"
#include "../headers/entities/Entity.h"
#include "../headers/entities/Tetrahedron.h"
#include "../headers/entities/Cube2Tex.h"
#include "../headers/entities/Cube3Tex.h"
#include "../headers/entities/ColoredEllipse.h"
#include "../headers/entities/Skybox.h"
#include "../headers/entities/SolarSystem.h"
#include "meshes/Mesh.h"
#include "meshes/DynamicMesh.h"
#include "Camera.h"

#include "pch.h"

class App {
private:
	sf::Clock clock;
	Camera camera;

	float elapsedTime;
	float deltaTime;

	bool isCamDirty = false;
	glm::vec2 mousePos;
	glm::vec2 mouseDelta;

	void SetIcon(sf::Window* wnd)
	{
		sf::Image image;
		image.create(16, 16);
		for (int i = 0; i < 16; ++i)
			for (int j = 0; j < 16; ++j)
				image.setPixel(i, j, { (uint8_t)(i * 16), (uint8_t)(j * 16), 0 });

		wnd->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	}

	GLuint Program;
	bool is_left = false,
		is_right = false,
		is_up = false,
		is_down = false,
		is_cam_active = false,

		decrease_ratio = false,
		increase_ratio = false,
		increase_zOffset = false,
		decrease_zOffset = false;

	int cur_task = 0;
	std::vector<Entity*> entities;
	Entity* skybox;
	SolarSystem* solar_system;
public:
	sf::Music* music;
	bool is_playing = false;
	void ResetClock() {
		deltaTime = clock.getElapsedTime().asSeconds();
		elapsedTime += deltaTime;
		clock.restart();
	}
	const float getDeltaTime() {
		return deltaTime;
	}
	App() {}
	void Init() {
		skybox = new Skybox();
		solar_system = new SolarSystem(50);
		solar_system->LoadModels("tree.obj", "vodica.jpg", "krosh.obj", "krosh.png");
		solar_system->PrepareData(glm::vec3(0.0f, -1.0f, 0.0f));
		entities.push_back(new Tetrahedron());
		entities.push_back(new Cube2Tex());
		entities.push_back(new Cube3Tex());
		entities.push_back(new ColoredEllipse());
		entities.push_back(new Mesh("tree.obj"));
		entities.push_back(new DynamicMesh("tree.obj"));
		music = new sf::Music();
		music->openFromFile("smesharikiBASSBOOSTED.wav");
		music->setVolume(100.0f);
	}
	void Draw() {
		if (cur_task >= entities.size()) return;
		auto r = 5.f;
		if (is_playing && music->getStatus() != sf::SoundSource::Playing)
			music->play();
		//entities[1]->Draw(glm::translate(glm::mat4(1.0f), glm::vec3(r * cos(elapsedTime * 0.4f), -3.f, r * sin(elapsedTime * 0.4f))) * glm::rotate(glm::mat4(1.0f), glm::radians(elapsedTime * 50.f), glm::vec3(0.0f, 1.0f, 0.f)), camera.GetViewMatrix(), camera.GetProjectionMatrix());
		//entities[5]->Draw(glm::rotate(glm::mat4(1.0f), glm::radians(elapsedTime * 10.f), glm::vec3(0.0f, 1.0f, 0.f)), camera.GetViewMatrix(), camera.GetProjectionMatrix());
		solar_system->Draw(elapsedTime, camera.GetViewMatrix(), camera.GetProjectionMatrix(), music->getPlayingOffset().asSeconds());
		// skybox should be rendered last for optimization
		skybox->Draw(glm::mat4(1.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix());
	}
	void Release() {
		if (cur_task >= entities.size()) return;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		for (auto& ent : entities) {
			ent->Release();
		}
	}
	void PollEvents(sf::Window& window) {
		sf::Event event;
		mouseDelta.x = 0;
		mouseDelta.y = 0;

		// We could have just check for cur_task. But in this case we must rely on order of entities - sad
		auto testT = dynamic_cast<Tetrahedron*>(entities[cur_task]);
		auto testMC = dynamic_cast<MixedCube*>(entities[cur_task]);
		auto testCE = dynamic_cast<ColoredEllipse*>(entities[cur_task]);
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{

				case sf::Keyboard::Num1:
					cur_task = 0;
					break;

				case sf::Keyboard::Num2:
					cur_task = 1;
					break;

				case sf::Keyboard::Num3:
					cur_task = 2;
					break;

				case sf::Keyboard::Num4:
					cur_task = 3;
					break;

				case sf::Keyboard::Num5:
					cur_task = 4;
					break;

				case sf::Keyboard::Num6:
					cur_task = 5;
					break;

				case sf::Keyboard::W:
					is_up = true;
					break;

				case sf::Keyboard::A:
					is_left = true;
					break;

				case sf::Keyboard::S:
					is_down = true;
					break;

				case sf::Keyboard::D:
					is_right = true;
					break;

				case sf::Keyboard::C:
					is_cam_active = true;
					break;

				case sf::Keyboard::Q:
					is_playing = true;
					break;

				default:
					break;
				}
			}
			else if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					is_up = false;
					break;

				case sf::Keyboard::A:
					is_left = false;
					break;

				case sf::Keyboard::S:
					is_down = false;
					break;

				case sf::Keyboard::D:
					is_right = false;
					break;

				case sf::Keyboard::C:
					is_cam_active = false;
					break;

				default:
					break;
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				camera.ProcessMouseScroll(event.mouseWheel.delta);
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (!isCamDirty) {
					mousePos.x = event.mouseMove.x;
					mousePos.y = event.mouseMove.y;
					isCamDirty = true;
				}
				mouseDelta.x = event.mouseMove.x - mousePos.x;
				mouseDelta.y = event.mouseMove.y - mousePos.y;
				mousePos.x = event.mouseMove.x;
				mousePos.y = event.mouseMove.y;
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		if (is_left)  camera.ProcessKeyboard(Camera::LEFT, deltaTime);
		if (is_right) camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
		if (is_up)    camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
		if (is_down)  camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
		if (is_cam_active) camera.ProcessMouseMovement(mouseDelta);

		if (cur_task >= entities.size()) return;

		auto velocity = entities[cur_task]->velocity;
		if (testT != nullptr || testMC != nullptr) {
			if (increase_zOffset) {
				entities[cur_task]->offset.z -= velocity;
			}
			if (decrease_zOffset) {
				entities[cur_task]->offset.z += velocity;
				entities[cur_task]->offset.z = std::min(-1.f, entities[cur_task]->offset.z);
			}
		}
		if (testMC != nullptr) {
			if (decrease_ratio)  testMC->AltMixRatio(-testMC->mixRatioStep);
			if (increase_ratio)  testMC->AltMixRatio(testMC->mixRatioStep);
			return;
		}
		if (testCE != nullptr) {
			if (is_left)  testCE->scale[0] = std::max(-1.f, testCE->scale[0] - velocity);
			if (is_right) testCE->scale[0] = std::min(1.f, testCE->scale[0] + velocity);
			if (is_up)    testCE->scale[1] = std::min(1.f, testCE->scale[1] + velocity);
			if (is_down)  testCE->scale[1] = std::max(-1.f, testCE->scale[1] - velocity);
			return;
		}
	}
};

#endif // !__MAIN_H__
