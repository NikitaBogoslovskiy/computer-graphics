#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <algorithm>
#include "pch.h"
#include "geometry/methods/linal.h"

class Camera
{
	/* to make it possible to wander through scene */
	Eigen::Matrix4f _view;
	ImVec3 _eye;
	ImVec3 _prev_eye;
	ImVec3 _pyr;
	ImVec3 _prev_pyr;
	ImVec3 _target;

	ImVec3 _direction;
	ImVec2 _rotation;
	ImVec3 _up;
	float _sensitivity;
	float _speed;

	bool _isMouseDirty;

	/* someday i will divide cam classes by projection types. someday. */
	Eigen::Matrix4f _projection;

	/* perspective */
	float _zFocus;
	ImVec2 _viewport; /*width, height of screen we project picture on! used in FoV perspective*/

	/* axonometry */
	float _angleX; float _angleY;

	Eigen::Matrix4f _scale; // scaling image
	ImDrawList* _draw_list; // draw list camera projects picture on
	int _mode;

	inline void resetPerspectiveSettings() {
		this->_viewport = ImVec2(800.f, 800.f); // maybe we'll need it someday
		this->_zFocus = 1000.f;
	}

	inline void resetAxonometrySettings() {
		this->_angleX = 145.f;
		this->_angleY = 135.f;
	}

public:
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
	inline float& sensitivity() { return this->_sensitivity; }
	inline float& speed() { return this->_speed; }
	inline bool& dirtiness() { return this->_isMouseDirty; }
	inline float& zFocus() { return this->_zFocus; }
	inline float& angleX() { return this->_angleX; }
	inline float& angleY() { return this->_angleY; }
	inline ImVec3& target() { return this->_target; }

	inline ImVec3& pitchYawRoll() { return this->_pyr; }
	inline void setPitchYawRoll(const ImVec2& deltaMouse) {
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
	}
	inline bool pitchYawRollChanged() {
		return this->_pyr != this->_prev_pyr;
	}

	inline const ImVec3& eye() { return this->_eye; }
	void setEye(const ImVec3& newEye) {
		if (Linal::len(this->_target - newEye) < 400.f) {
			return;
		}
		this->_eye = newEye;
	}
	inline bool eyeChanged() {
		return this->_eye != this->_prev_eye;
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

		this->_prev_pyr = this->_pyr;
	}
	
	inline const ImVec3 direction() { return this->_target - this->_eye; /*return this->_direction;*/ }
	inline const ImVec3& up() { return this->_up; }
	inline const ImVec2& viewport() { return this->_viewport; }
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
		//return this->_scale * projection() * this->_view;
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

	inline void resetPosition(const ImVec3& eye = ImVec3(0.f, 0.f, 400.f), const ImVec3& pitchYawRoll = ImVec3(0.f, 90.f, 0.f), const ImVec3& target = ImVec3(0.f, 0.f, 0.f), const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
		setEyeAndPYR(eye, pitchYawRoll);
		this->_prev_pyr = pitchYawRoll;
		this->_target = target;
		this->_up = up;
		updateEyeRotation();
	}

	void setEyeAndPYR(const ImVec3& eye = ImVec3(0.f, 0.f, 400.f), const ImVec3& pitchYawRoll = ImVec3(0.f, 90.f, 0.f)) {
		this->_eye = eye;
		this->_pyr = pitchYawRoll;
	}

	inline void updateLook() {
		lookAt(this->_target);
	}

	inline void lookAt(const ImVec3& target) {
		this->_view = Linal::lookAt(this->_eye, target, this->_up);
	}

	inline void altPerspectiveScale(const float& d = 1.f) {
		for (int i = 0; i < 3; i++) {
			if (this->_scale(i, i) + d < 1.f) continue;
			this->_scale(i, i) += d;
		}
	}
};

#endif
