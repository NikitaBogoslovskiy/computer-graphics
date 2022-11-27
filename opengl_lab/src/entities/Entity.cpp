#include "../../headers/entities/Entity.h"

void Entity::ReleaseShader() {
	// ��������� ����, �� ��������� ��������� ���������
	glUseProgram(0);
	// ������� ��������� ���������
	glDeleteProgram(Program);
}

void Entity::Init() {
	InitShader();
	InitVO();
}

void Entity::Release() {
	// �������
	ReleaseShader();
	// ��������� �����
	ReleaseVO();
}