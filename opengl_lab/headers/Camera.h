#pragma once

#include "pch.h"

class Player;

class Camera {

	glm::vec3 Position; //in world coordinates

	glm::vec3 Front;
	glm::vec3 Right;

	glm::vec3 Up;
	glm::vec3 WorldUp;

	float Yaw; // rotation about Y axis
	float Pitch; // rotation about X axis

	float Velocity;
	float MouseSensitivity;
	float FOV_Angle;

	void updateCameraVectors()
	{
		Front = glm::normalize(glm::vec3(cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)), sin(glm::radians(Pitch)), sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))));
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	glm::mat4 view;
	glm::mat4 projection;

	bool needsUpdateView = true;
	bool needsUpdateProjection = true;

	Player* player;

public:
	enum Direction {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	Camera(const glm::vec3& position = glm::vec3(0.0f, 2.5f, 5.0f),
		const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f))
		: Velocity(5.f), MouseSensitivity(0.1f), FOV_Angle(45.f), Yaw(-90.f), Pitch(-10.f)
	{
		Position = position;
		WorldUp = worldUp;
		Front = front;
		updateCameraVectors();
	}

	inline const glm::mat4& GetViewMatrix()
	{
		if (needsUpdateView) {
			view = glm::lookAt(Position, Position + Front, Up);
			needsUpdateView = false;
		}
		return view;
	}

	inline const glm::mat4& GetProjectionMatrix()
	{
		if (needsUpdateProjection) {
			projection = glm::perspective(glm::radians(FOV_Angle), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
			needsUpdateProjection = false;
		}
		return projection;
	}

	const inline glm::vec3& GetPosition() const {
		return Position;
	}

	const inline glm::vec3& GetDirection() const {
		return Front;
	}

	// ================================================================== interface api

	void ProcessKeyboard(Direction direction, const float& deltaTime)
	{
		float velocity = Velocity * deltaTime;

		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;

		needsUpdateView = true;
	}

	void ProcessMouseMovement(const glm::vec2& offset)
	{
		Yaw += offset.x * MouseSensitivity;
		Pitch -= offset.y * MouseSensitivity;

		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;

		updateCameraVectors();

		needsUpdateView = true;
	}

	void ProcessMouseScroll(const float& scroll)
	{
		FOV_Angle -= scroll * 10.f;
		if (FOV_Angle < 1.0f) FOV_Angle = 1.0f;
		if (FOV_Angle > 180.0f) FOV_Angle = 180.0f;

		needsUpdateProjection = true;
	}
};