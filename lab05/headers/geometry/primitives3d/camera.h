#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "pch.h"
#include "geometry/methods/linal.h"

class Camera
{
	/* to make it possible to wander through scene */
	Eigen::Matrix4f _view;
	ImVec3 _eye;
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

	float _zNear;
	float _zFar;
	float _aspectRatio;
	float _FoV;
	ImVec2 _viewport; /*width, height of screen we project picture on! used in FoV perspective*/

	/* axonometry */
	float _angleX; float _angleY;

	Eigen::Matrix4f _scale; // scaling image
	ImDrawList* _draw_list; // draw list camera projects picture on
	int _mode;

	inline void resetPerspectiveSettings() {
		this->_zFocus = 1000.f;

		this->_viewport = ImVec2(800.f, 800.f); // maybe we'll need it someday
		this->_zNear = 1.f;
		this->_zFar = 10000.f;
		this->_aspectRatio = 1.f;
		this->_FoV = 100.f;
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

	Camera(const ImVec2& viewport, const ImVec3& eye, Eigen::Matrix4f& projection, ImDrawList*& draw_list) {
		this->_projection = projection;
		this->_viewport = viewport;
		this->_eye = eye;
		this->_draw_list = draw_list;
	}
	inline int& mode() { return this->_mode; }
	inline float& sensitivity() { return this->_sensitivity; }
	inline float& speed() { return this->_speed; }
	inline bool& dirtiness() { return this->_isMouseDirty; }

	inline ImVec3& eye() { return this->_eye; }
	inline ImVec3& direction() { return this->_direction; }
	inline ImVec2& rotation() { return this->_rotation; }
	inline ImVec3& up() { return this->_up; }

	inline ImVec2& viewport() { return this->_viewport; }
	inline float& zFocus() { return this->_zFocus; }

	inline float& angleX() { return this->_angleX; }
	inline float& angleY() { return this->_angleY; }

	inline Eigen::Matrix4f& scale() { return this->_scale; }
	inline Eigen::Matrix4f& view() { return this->_view; } 

	inline Eigen::Matrix4f projection() {
		switch ((CamMode)this->_mode) {
		case CamMode::Perspective:
			//return Linal::perspective(this->_zFocus);
			return Linal::perspectiveFoVDirectX(this->_FoV, this->_aspectRatio, this->_zNear, this->_zFar);
		case CamMode::Axonometry:
			return Linal::axonometry(this->_angleX, this->_angleY);
		default:
			return Affine::identity();
		}
	}

	inline Eigen::Matrix4f viewProjecion() {
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
		updatePosition();
		updateLook();

		this->_scale = Affine::identity();
	}

	inline void updatePosition(const ImVec3& eye = ImVec3(0.f, 0.f, 10.f), const ImVec2& rotation = ImVec2(-90.f, 0.f), const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
		this->_eye = eye;
		this->_rotation = rotation;
		this->_up = up;
		updateDirection();
	}

	inline void updateDirection() {
		auto radYaw = DegreesToRadians(this->_rotation.x);
		auto radPitch = DegreesToRadians(this->_rotation.y);

		ImVec3 newDirection;
		newDirection.x = cos(radYaw) * cos(radPitch);
		newDirection.y = sin(radPitch);
		newDirection.z = sin(radYaw) * cos(radPitch);
		this->_direction = Linal::normalize(newDirection);
	}

	inline void updateLook() {
		lookAt(this->_eye + this->_direction);
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
