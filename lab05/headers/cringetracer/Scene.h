#pragma once
#include <thread>

#include "Image.h"

class Scene
{
public:
	Scene();
	~Scene();
	void Render(Image& outputImage);
	void SubRender(Image& img, const size_t start, const size_t end, const size_t xSize, const size_t ySize);
};
