#pragma once
#include <algorithm>
#include "../pch.h"
#include "../meshes/Bullet.h"

class Player
{
	glm::vec3 Position; //in world coordinates

	glm::vec3 Front;
	glm::vec3 Right;

	glm::vec3 Up;
	glm::vec3 WorldUp;

	// I LIKE TO CONTAINT IT BY NAMES.
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
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));

		//printf("%lf %lf %lf\n", Front.x, Front.y, Front.z);
	}

	float VELOCITY = 1.F;
	float TURN_VELOCITY = 100.F;

	PartedIllumiMesh* _mesh;

	void updateMeshPosition() {
		if (!_mesh) return;
		_mesh->position = Position;
		auto bumper = Position + 0.008f * Front + glm::vec3(0.0, 0.13, 0.0);
		hl[0].position = bumper + 0.2f * Right;
		hl[1].position = bumper - 0.2f * Right;
		//printf("%lf %lf %lf\n", Position.x, Position.y, Position.z);
	}
	void updateMeshRotation() {
		if (!_mesh) return;
		_mesh->rotation = glm::vec3(Pitch, Yaw, Roll);
		hl[0].direction = Front;
		hl[1].direction = Front;
	}

public:

	std::vector<Bullet*> mag;

	inline void TryKill(std::vector<PartedIllumiMesh*>& enemies) {
		for (auto& b : mag) {
			if (b->inMag) continue;
			int enemy_ind = -1;
			for (size_t i = 0; i < enemies.size(); i++) {
				auto collided = b->CheckCollision(enemies[i]);
				if (!collided) continue;
				enemy_ind = i; break;
			}
			if (enemy_ind == -1) continue;
			b->inMag = true;

			//enemies.clear();
			//using std::swap;
			//swap(enemies[enemy_ind], enemies.back());
			//enemies.pop_back();
			//printf("collision: %lf %lf %lf; %d\n", enemies[enemy_ind]->position.x, enemies[enemy_ind]->position.y, enemies[enemy_ind]->position.z, enemies.size());
			enemies.erase(enemies.begin() + enemy_ind);
			//
		}
	}

	inline void UpdateBullets(const float& deltaTime) {
		for (auto& b : mag) {
			b->Move(deltaTime);
		}
	}

	inline void MakeShot() {
		printf("searching for bullet\n");
		int ind = -1;
		for (size_t i = 0; i < mag.size(); i++) {
			if (!mag[i]->inMag) continue;
			ind = i; break;
		}
		if (ind == -1) return;
		printf("found bullet: %d\n", ind);
		mag[ind]->position = Position + glm::vec3{ 0.f, 0.1f, 0.f };
		mag[ind]->Direction = Front;
		mag[ind]->inMag = false;
	}
	SpotLight hl[2];

	enum Direction { FORWARD, BACKWARD, LEFT, RIGHT };

	inline void SwitchHeadlights() {
		hl[0].intensity = !hl[0].intensity;
		hl[1].intensity = !hl[1].intensity;
	}

	inline void SetMesh(PartedIllumiMesh* mesh) { _mesh = mesh; }

	inline void ResetVelocity() {
		VELOCITY = 1.F;
		TURN_VELOCITY = 100.F;
	}

	inline void IncVelocity() {
		VELOCITY += 1.f;
		TURN_VELOCITY += 50.f;
	}

	inline void DecVelocity() {
		auto newVel = VELOCITY - 1.f;
		if (newVel <= 0.f) return;
		VELOCITY = newVel;

		auto newTurnVel = TURN_VELOCITY - 50.f;
		if (newTurnVel <= 0.f) return;
		TURN_VELOCITY = newTurnVel;
	}

	Player(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f)) : Yaw(0.f)
	{
		Position = position;
		WorldUp = worldUp;
		updateVectors();
		updateMeshPosition();
		updateMeshRotation();
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

		updateMeshPosition();
		updateMeshRotation();
	}
};
