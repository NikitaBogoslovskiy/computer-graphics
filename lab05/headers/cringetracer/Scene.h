#pragma once

#include <thread>
#include "Image.h"
#include "GeometricBodies/GeometricBody.h"

class Scene
{
public:
	std::vector<GeometricBody*> bodies;
	Scene();
	~Scene();
	void Render(Image& outputImage);
	void SubRender(Image& img, const size_t start, const size_t end, const size_t xSize, const size_t ySize);
};
