#pragma once
#include "../Camera.h"
#include "Player.h"

class Camera3P : public Camera // third person camera
{
	Player* _player;
	float Pitch;
	float Yaw;
	float R;
	float MouseSensitivity;

	virtual void updateCameraVectors() override {
		Front = glm::normalize(_target() - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void updateEyePosition() {
		auto pitchRad = glm::radians(Pitch);
		auto yawRad = glm::radians(Yaw);

		auto cosPitch = cos(pitchRad);

		this->Position = _target() + R * glm::vec3(
			cosPitch * cos(yawRad),
			sin(pitchRad),
			cosPitch * sin(yawRad)
		);

		needsUpdateView = true;
	}

	const float MIN_R = 1.F;
	const  glm::vec3  OFFSET_POINT = glm::vec3(0.f, 0.5f, 0.f);

	inline glm::vec3 _target() const {
		return _player->GetPosition() + OFFSET_POINT;
	}

public:

	inline void SetPlayer(Player* player) { _player = player; }

	virtual inline const glm::mat4& GetViewMatrix() override
	{
		if (needsUpdateView) {
			view = glm::lookAt(Position, _target(), Up);
			needsUpdateView = false;
		}
		return view;
	}

	Camera3P(Player* player, const float r = 2.f, const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f))
		: R(r), Yaw(0.f), Pitch(30.f), MouseSensitivity(0.1f) {
		_player = player;
		WorldUp = worldUp;
		FOV_Angle = 45.f;

		updateEyePosition();
		updateCameraVectors();
	}

	// ================================================================== interface api

	virtual void ProcessMouseMovement(const glm::vec2& offset) override
	{
		Yaw += offset.x * MouseSensitivity;
		Pitch -= offset.y * MouseSensitivity;

		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;

		updateEyePosition();
		updateCameraVectors();

		needsUpdateView = true;
	}

	virtual void ProcessMouseScroll(const float& scroll) override
	{
		auto newR = R - scroll;
		R = std::max(newR, MIN_R);

		updateEyePosition();
		updateCameraVectors(); // just in case. the direction of front stays the same

		needsUpdateView = true;
	}
};
