#pragma once
#include "../pch.h"
#include "PartedIllumiMesh.h"
//#include "../game/Player.h"

class Bullet : public PartedIllumiMesh
{

protected:
	float collisionR = 0.1f;
	float ACCELERATION = 10.F;
public:

	bool inMag = true;
	glm::vec3 Direction;

	inline void Move(const float& deltaTime) {
		if (inMag) return;
		position += Direction * ACCELERATION * deltaTime;
		if (glm::length(position) >= 6.f) inMag = true;
	}

	inline bool CheckCollision(PartedIllumiMesh* pim) {
		return glm::length(pim->position - position) <= collisionR;
	}

	Bullet() {
	}
	~Bullet() {
	}

private:

};