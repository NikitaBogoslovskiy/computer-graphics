#include "../../headers/entities/Entity.h"

void Entity::ReleaseShader() {
	// ��������� ����, �� ��������� ��������� ���������
	glUseProgram(0);
	// ������� ��������� ���������
	glDeleteProgram(Program);
}

void Entity::Release() {
	// �������
	ReleaseShader();
	// ��������� �����
	ReleaseVO();
}