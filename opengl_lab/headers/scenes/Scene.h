#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

template<class T>
class Scene {
	std::vector<T&> objects;

public:
	Scene(const std::initializer_list<T&>& drawables);

	void Draw();
};

#endif // !SCENE_H