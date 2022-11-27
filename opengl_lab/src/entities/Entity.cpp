#include "../../headers/entities/Entity.h"

void Entity::ReleaseShader() {
	// Передавая ноль, мы отключаем шейдерную программу
	glUseProgram(0);
	// Удаляем шейдерную программу
	glDeleteProgram(Program);
}

void Entity::Init() {
	InitShader();
	InitVO();
}

void Entity::Release() {
	// Шейдеры
	ReleaseShader();
	// Вершинный буфер
	ReleaseVO();
}