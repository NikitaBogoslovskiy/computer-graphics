#pragma once

#include <thread>
#include "Image.h"
#include "GeometricBodies/GeometricBody.h"
#include "Lights/Light.h"
#include "Materials/MtlLib.h"

class Scene
{
public:
	std::vector<GeometricBody*> bodies;
	std::vector<Light*> lights;
	//std::map<std::string, Material*> materials;
	MtlLib materials;
	Scene();
	~Scene();
};
