#pragma once

#include "../pch.h"
#include "../meshes/PartedIllumiMesh.h"

class Player
{
	glm::vec3 Position; //in world coordinates

	glm::vec3 Front;
	glm::vec3 Right;

	glm::vec3 Up;
	glm::vec3 WorldUp;

	float Yaw;
	const float Pitch = 0.0; // just in case
	const float Roll = 0.0;

	void updateVectors()
	{
		Front = glm::normalize(glm::vec3(
			-cos(glm::radians(Yaw)),
			sin(glm::radians(Pitch)), // 0.0
			sin(glm::radians(Yaw))
		));

		printf("%lf %lf %lf\n", Front.x, Front.y, Front.z);
	}

	float VELOCITY = 1.F;
	float TURN_VELOCITY = 100.F;

	PartedIllumiMesh* _mesh;

	void UpdateMeshPosition() {
		_mesh->position = Position;
	}
	void UpdateMeshRotation() {
		_mesh->rotation = glm::vec3(Pitch, Yaw, Roll);
	}
public:
	void IncVelocity() {
		VELOCITY += 1.f;
	}

	void DecVelocity() {
		auto newVel = VELOCITY - 1.f;
		VELOCITY = std::max(newVel, 0.f);
	}
	void SetMesh(PartedIllumiMesh* mesh) { _mesh = mesh; }

	enum Direction { FORWARD, BACKWARD, LEFT, RIGHT };

	Player(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f)) : Yaw(0.f)
	{
		Position = position;
		WorldUp = worldUp;
		Front = front;
		updateVectors();
	}

	const inline glm::vec3& GetPosition() const { return Position; }
	const inline glm::vec3& GetDirection() const { return Front; }

	// ================================================================== interface api

	void ProcessKeyboard(Direction direction, const float& deltaTime)
	{
		float velocity = VELOCITY * deltaTime;
		float turnVelocity = TURN_VELOCITY * deltaTime;

		switch (direction)
		{
		case Player::FORWARD:
			Position += Front * velocity;
			break;

		case Player::BACKWARD:
			Position -= Front * velocity;
			break;

		case Player::LEFT: {
			Yaw += turnVelocity;
			updateVectors();
		} break;

		case Player::RIGHT: {
			Yaw -= turnVelocity;
			updateVectors();
		} break;

		default:
			break;
		}

		UpdateMeshPosition();
		UpdateMeshRotation();
	}
};
