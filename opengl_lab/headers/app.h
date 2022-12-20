#pragma once
#ifndef APP_H
#define APP_H

#include "pch.h"
#include <iostream>
#include "shader_loader.h"
#include "../headers/entities/Skybox.h"
#include "../headers/scenes/LightExhibition.h"
#include "../headers/scenes/SolarSystem.h"
#include "Camera.h"

class App {
	sf::Window* window;

	sf::Clock clock;
	Camera camera;

	float elapsedTime;
	float deltaTime;

	bool isCamDirty = false;
	glm::vec2 mousePos;
	glm::vec2 mouseDelta;

	/*
	void SetIcon(sf::Window* wnd)
	{
		sf::Image image;
		image.create(16, 16);
		for (int i = 0; i < 16; ++i)
			for (int j = 0; j < 16; ++j)
				image.setPixel(i, j, { (uint8_t)(i * 16), (uint8_t)(j * 16), 0 });

		wnd->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	}
	*/

	GLuint Program;
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
	} settings;

	int cur_scene;
	std::vector<Scene*> scenes;
public:
	bool is_playing = false;
	void ResetClock();
	inline const float& getDeltaTime() const {
		return deltaTime;
	}
	inline Scene* currScene() { return scenes[cur_scene]; }
	App(sf::Window* _window) { window = _window; }
	void Init();
	void Draw();
	void Release();
	void PollEvents(sf::Window& window);
};

#endif // !APP_H
