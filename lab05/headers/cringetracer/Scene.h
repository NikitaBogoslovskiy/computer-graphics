#pragma once

#include <thread>
#include "Image.h"
#include "GeometricBodies/GeometricBody.h"
#include "Lights/Light.h"

class Scene
{
public:
	std::vector<GeometricBody*> bodies;
	std::vector<Light*> lights;
	// lights
	// and so on

	Scene();
	~Scene();
};
