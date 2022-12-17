#include "../headers/scenes/Scene.h"

template<class T>
inline Scene<T>::Scene(const std::initializer_list<T&>& drawables){
	objects.push_back(drawables);
}

template<class T>
void Scene<T>::Draw(){
	for (T& o : objects) {
		o.Draw();
	}
}
