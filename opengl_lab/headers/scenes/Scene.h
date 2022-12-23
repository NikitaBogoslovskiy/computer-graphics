#ifndef SCENE_H
#define SCENE_H

#include "../pch.h"
#include "../Camera.h"
#include "../entities/Skybox.h"
class Scene {
public:
	Entity* skybox;
	virtual inline void Draw(float time_coefficient, Camera& cam) {};
};

#endif // !SCENE_H