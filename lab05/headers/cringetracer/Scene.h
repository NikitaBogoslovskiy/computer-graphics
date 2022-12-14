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
	inline static MtlLib materials;
	Scene();
	~Scene();

	static void FillExampleScene1(Scene* inScene);
	static void FillExampleScene2(Scene* inScene);
};
