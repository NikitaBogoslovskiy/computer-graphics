#ifndef __MAIN_H__
#define __MAIN_H__

#include "gl/glew.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "shader_loader.h"
#include "../headers/entities/Entity.h"
#include "../headers/entities/Tetrahedron.h"
#include "../headers/entities/Cube2Tex.h"
#include "../headers/entities/Cube3Tex.h"
#include "../headers/entities/ColoredEllipse.h"

class App {
private:
	sf::Clock clock;

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

		decrease_ratio = false,
		increase_ratio = false,
		increase_zOffset = false,
		decrease_zOffset = false;

	int cur_task = 0;
	std::vector<Entity*> entities;

public:
	App() {}
	void Init() {
		entities.push_back(new Tetrahedron());
		entities.push_back(new Cube2Tex());
		entities.push_back(new Cube3Tex());
		entities.push_back(new ColoredEllipse());
	}
	void Draw() {
		if (cur_task >= entities.size()) return;
		float time = clock.getElapsedTime().asSeconds();
		entities[cur_task]->Draw(time);
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
				case sf::Keyboard::Left:
					is_left = true;
					break;

				case sf::Keyboard::Right:
					is_right = true;
					break;

				case sf::Keyboard::Up:
					is_up = true;
					break;

				case sf::Keyboard::Down:
					is_down = true;
					break;

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

				case sf::Keyboard::A:
					decrease_ratio = true;
					break;

				case sf::Keyboard::D:
					increase_ratio = true;
					break;

				case sf::Keyboard::W:
					increase_zOffset = true;
					break;

				case sf::Keyboard::S:
					decrease_zOffset = true;
					break;
				default:
					break;
				}
			}
			else if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					is_left = false;
					break;

				case sf::Keyboard::Right:
					is_right = false;
					break;

				case sf::Keyboard::Up:
					is_up = false;
					break;

				case sf::Keyboard::Down:
					is_down = false;
					break;

				case sf::Keyboard::A:
					decrease_ratio = false;
					break;

				case sf::Keyboard::D:
					increase_ratio = false;
					break;

				case sf::Keyboard::W:
					increase_zOffset = false;
					break;

				case sf::Keyboard::S:
					decrease_zOffset = false;
					break;

				default:
					break;
				}
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		if (cur_task >= entities.size()) return;
		
		auto velocity = entities[cur_task]->velocity;
		if (testT != nullptr) {
			if (is_left)  entities[cur_task]->offset[0] = std::max(-1.f, entities[cur_task]->offset[0] - velocity);
			if (is_right) entities[cur_task]->offset[0] = std::min(1.f, entities[cur_task]->offset[0] + velocity);
			if (is_up)    entities[cur_task]->offset[1] = std::min(1.f, entities[cur_task]->offset[1] + velocity);
			if (is_down)  entities[cur_task]->offset[1] = std::max(-1.f, entities[cur_task]->offset[1] - velocity);
			return;
		}
		if (testMC != nullptr) {
			if (decrease_ratio)  testMC->AltMixRatio(-testMC->mixRatioStep);
			if (increase_ratio)  testMC->AltMixRatio(testMC->mixRatioStep);
			if (increase_zOffset) {
				testMC->zOffset -= velocity;
			}
			if (decrease_zOffset) {
				testMC->zOffset += velocity;
				testMC->zOffset = std::min(-1.f, testMC->zOffset);
			}
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
