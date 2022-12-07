#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <algorithm>
#include "pch.h"
#include "geometry/methods/linal.h"

class Camera
{
	Eigen::Matrix4f _view;
	Eigen::Matrix4f _projection;
	Eigen::Matrix4f _scale;

	ImVec3 _eye;
	ImVec3 _target;
	ImVec3 _up;

	ImVec3 _direction;
	ImVec3 _pyr;

	float _sensitivity;
	float _speed;

	bool _isMouseDirty;

	float _zFocus;

	float _angleX; float _angleY;

	int _mode;

	inline void resetPerspectiveSettings() {
		this->_zFocus = 1000.f;
	}

	inline void resetAxonometrySettings() {
		this->_angleX = 145.f;
		this->_angleY = 135.f;
	}

	void setEye(const ImVec3& newEye) {
		if (Linal::len(this->_target - newEye) < 5.f) return;
		this->_eye = newEye;

		updateLook();
	}

	inline void updateLook() {
		this->_view = Linal::lookAt(this->_eye, this->_target, this->_up);
	}

	void updateEyeRotation() {
		auto r = Linal::len(direction());
		auto pitchRad = DegreesToRadians(this->_pyr.x);
		auto yawRad = DegreesToRadians(this->_pyr.y);

		auto cosPitch = cos(pitchRad);
		auto sinPitch = sin(pitchRad);
		auto cosYaw = cos(yawRad);
		auto sinYaw = sin(yawRad);

		this->_eye.x = r * cosPitch * cosYaw;
		this->_eye.y = r * sinPitch;
		this->_eye.z = r * cosPitch * sinYaw;

		updateLook();
	}

public:
	enum Direction {
		Forward,
		Backward,
		Left,
		Right
	};

	enum CamMode {
		Perspective,
		Axonometry,
	};
	static CamMode cameraModes;

	Camera() {
		resetFlightSettings();
		resetCamPosition();
		resetPerspectiveSettings();
		resetAxonometrySettings();
	}
	inline int& mode() { return this->_mode; }
	inline bool& dirtiness() { return this->_isMouseDirty; }

	inline const ImVec3& eye() { return this->_eye; }
	inline ImVec3& target() { return this->_target; }
	inline const ImVec3& up() { return this->_up; }
	inline const ImVec3 direction() { return this->_target - this->_eye; }

	inline const ImVec3& pitchYawRoll() { return this->_pyr; }

	inline float& zFocus() { return this->_zFocus; }
	inline float& angleX() { return this->_angleX; }
	inline float& angleY() { return this->_angleY; }

	inline const Eigen::Matrix4f& scale() { return this->_scale; }
	inline const Eigen::Matrix4f& view() { return this->_view; }
	inline const Eigen::Matrix4f projection() {
		switch ((CamMode)this->_mode) {
		case CamMode::Perspective:
			return Linal::perspective(this->_zFocus);
		case CamMode::Axonometry:
			return Linal::axonometry(this->_angleX, this->_angleY);
		default:
			return Affine::identity();
		}
	}
	inline const Eigen::Matrix4f viewProjecion() {
		switch ((CamMode)this->_mode) {
		case CamMode::Perspective:
			return this->_scale * projection() * (this->_view);
		case CamMode::Axonometry:
			return (this->_scale) * projection();
		default:
			return Affine::identity();
		}
	}

	inline void resetFlightSettings() {
		this->_sensitivity = 0.1f;
		this->_speed = 1000.f;
		this->_isMouseDirty = false;
	}

	inline void resetProjectionSettings() { // so we can save our current projection type settings 
		switch ((CamMode)this->_mode) {
		case CamMode::Perspective:
			resetPerspectiveSettings();
			break;
		case CamMode::Axonometry:
			resetAxonometrySettings();
			break;
		default:
			break;
		}
	}

	inline void resetCamPosition() {
		resetPosition();
		updateLook();

		this->_scale = Affine::identity();
	}

	inline void resetPosition(const ImVec3& eye = ImVec3(0.f, 0.f, 10.f), const ImVec3& pitchYawRoll = ImVec3(0.f, 90.f, 0.f), const ImVec3& target = ImVec3(0.f, 0.f, 0.f), const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
		this->_target = target;
		this->_up = up;
		setEyeAndPYR(eye, pitchYawRoll);
	}

	void setEyeAndPYR(const ImVec3& eye = ImVec3(0.f, 0.f, 10.f), const ImVec3& pitchYawRoll = ImVec3(0.f, 90.f, 0.f)) {
		this->_eye = eye;
		this->_pyr = pitchYawRoll;
		updateEyeRotation();
	}

	// ================================================================== interface api

	void ProcessKeyboard(Direction direction, const float& deltaTime)
	{
		float velocity = _speed * deltaTime;
		if (direction == Forward)
			this->setEye(_eye + velocity * Linal::normalize(this->direction()));
		else if (direction == Backward)
			this->setEye(_eye - velocity * Linal::normalize(this->direction()));
		//else if (direction == Left) 
		// Position -= Right * velocity;
		//else if (direction == Right) 
		// Position += Right * velocity;

	}

	inline void ProcessMouseMovement(const ImVec2& deltaMouse) {
		ImVec2 offset = this->_sensitivity * deltaMouse;

		this->_pyr.x += offset.y; // pitch
		this->_pyr.y += offset.x; // yaw

		this->_pyr.x = std::min(this->_pyr.x, 89.0f);
		this->_pyr.x = std::max(this->_pyr.x, -89.0f);

		if (this->_pyr.y >= 360.f) {
			this->_pyr.y = this->_pyr.y - 360.f;
		}
		if (this->_pyr.y <= -360.f) {
			this->_pyr.y = this->_pyr.y + 360.f;
		}
		updateEyeRotation();
	}

	inline void ProcessMouseScroll(const float x) {
		auto d = x < 0 ? -0.5f : 0.5f;
		for (int i = 0; i < 3; i++) {
			if (this->_scale(i, i) + d < 1.f) continue;
			this->_scale(i, i) += d;
		}
	}

};

#endif
