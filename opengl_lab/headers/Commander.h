#pragma once

#include "Camera.h"

class Commander : public Camera
{
public:
	inline void SetPosition(const glm::vec3& pos) {
		Position = pos;
	}

	inline void SetYaw(const float& angle) {
		Yaw = angle;
	}
};