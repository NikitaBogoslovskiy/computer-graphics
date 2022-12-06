#pragma once

#include <thread>
#include "Image.h"
#include "GeometricBodies/GeometricBody.h"

class Scene
{
public:
	std::vector<GeometricBody*> bodies;
	// lights
	// and so on

	Scene();
	~Scene();
};
