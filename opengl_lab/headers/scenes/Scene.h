#ifndef SCENE_H
#define SCENE_H

#include "../pch.h"
#include "../Camera.h"

class Scene {
public:
	virtual inline void Draw(float time_coefficient, Camera& cam) {};
};

#endif // !SCENE_H