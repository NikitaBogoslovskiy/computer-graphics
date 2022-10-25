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
	ImVec2 _viewport; /*width, height of screen we project picture on! used in FoV perspective*/

	/* axonometry */
	float _angleX; float _angleY;

	Eigen::Matrix4f _scale; // scaling image
	ImDrawList* _draw_list; // draw list camera projects picture on
public:
	Camera() {
		resetFlightSettings();
		resetCamPosition();
		setPerspective();
	}

	Camera(const ImVec2& viewport, const ImVec3& eye, Eigen::Matrix4f& projection, ImDrawList*& draw_list) {
		this->_projection = projection;
		this->_viewport = viewport;
		this->_eye = eye;
		this->_draw_list = draw_list;
	}

	inline float& sensitivity() { return this->_sensitivity; }
	inline float& speed() { return this->_speed; }
	inline bool& dirtiness() { return this->_isMouseDirty; }

	inline ImVec3& eye() { return this->_eye; }
	inline ImVec3& direction() { return this->_direction; }
	inline ImVec2& rotation() { return this->_rotation; }
	inline ImVec3& up() { return this->_up; }

	inline ImVec2& viewport() { return this->_viewport; }
	inline float& zFocus() { return this->_zFocus; }

	inline Eigen::Matrix4f& scale() { return this->_scale; }
	inline Eigen::Matrix4f& view() { return this->_view; }
	inline Eigen::Matrix4f& projection() { return this->_projection; }
	inline Eigen::Matrix4f viewProjecion() { return (this->_scale) * (this->_projection) * (this->_view); }
	
	inline void resetFlightSettings() {
		this->_sensitivity = 0.1f;
		this->_speed = 1000.f;
		this->_isMouseDirty = false;
	}

	inline void resetCamPosition() {
		updatePosition();
		updateLook();

		resetPerspectiveSettings();

		this->_scale = Affine::identity();
	}

	inline void updatePosition(const ImVec3& eye = ImVec3(0.f, 0.f, 1.f), const ImVec2& rotation = ImVec2(-90.f, 0.f), const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
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
		this->_view = Linal::lookAt(this->_eye, this->_eye + this->_direction, this->_up);
	}

	inline void lookAt(const ImVec3& target) {
		this->_view = Linal::lookAt(this->_eye, target, this->_up);
	}

	// PROJECTIONS

	inline void resetProjection() {
		this->_projection = Affine::identity();
	}

	// PERSPECTIVE
	inline void resetPerspectiveSettings() {
		this->_viewport = ImVec2(800.f, 800.f); // maybe we'll need it someday
		this->_zFocus = 1000.f;
	}
	/* idk what to use it for now */
	inline void setPerspectiveFoVProjection(const float& FoV, const float& ratio, const float& zNear, const float& zFar) {
		this->_projection = Linal::perspectiveFoV(FoV, ratio, zNear, zFar);
	}
	inline void setPerspective() {
		resetProjection();
		setPerspectiveZFocus(this->_zFocus);
	}
	inline void setPerspectiveXFocus(const float& xFocus) {
		this->_projection(3, 0) = -1 / xFocus;
	}
	inline void setPerspectiveYFocus(const float& yFocus) {
		this->_projection(3, 1) = -1 / yFocus;
	}
	inline void setPerspectiveZFocus(const float& zFocus) {
		this->_projection(3, 2) = -1 / zFocus;
	}
	inline void altPerspectiveScale(const float& d = 1.f) {
		for (int i = 0; i < 3; i++) {
			if (this->_scale(i, i) + d < 1.f) continue;
			this->_scale(i, i) += d;
		}
	}

	// AXONOMETRY

	inline void setAxonometryProjection(const float& angleX, const float& angleY) {
		this->_projection = Linal::axonometry(angleX, angleY);
	}

	inline void setAxonometryAngles(const float& angleX, const float& angleY) {
		this->_angleX = angleX;
		this->_angleY = angleY;
	}
};

#endif
