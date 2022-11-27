#ifndef __MAIN_H__
#define __MAIN_H__

#include "gl/glew.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "shader_loader.h"
#include "Entity.h"
#include "Tetrahedron.h"

class App {
private:
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


public:
	std::vector<Entity*> entities;
	int cur_task = 0;
	bool is_left = false,
		is_right = false,
		is_up = false,
		is_down = false;

	App() {}
	void Init() {
		entities.push_back(new Tetrahedron());
	}
	void Draw() {
		if (cur_task >= entities.size()) return;
		entities[cur_task]->Draw();
	}
	void Release() {
		if (cur_task >= entities.size()) return;
		entities[cur_task]->Release();
	}
};

//sf::Shader shader;
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//int width, height, nrChannels;
//unsigned char* data;

#endif // !__MAIN_H__
