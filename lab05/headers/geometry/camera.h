#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "pch.h"
#include "linal.h"

class Camera
{
	ImVec2 _viewport; /*width, height of screen we project picture on! */
	ImVec3 _eye;
	ImVec3 _direction;
	ImVec3 _rotation;
	ImVec3 _up;
	Eigen::Matrix4f _view;
	Eigen::Matrix4f _projection;
	Eigen::Matrix4f _vp;
	Eigen::Matrix4f _scale;
	float _zFocus;
	ImDrawList* _draw_list;
	bool _isMouseDirty;
public:
	Camera() {
		resetPosition();
	}

	Camera(const ImVec2& viewport, const ImVec3& eye, Eigen::Matrix4f& projection, ImDrawList*& draw_list) {
		this->_projection = projection;
		this->_viewport = viewport;
		this->_eye = eye;
		this->_draw_list = draw_list;
	}

	inline ImVec2& viewport() { return this->_viewport; }
	inline ImVec3& eye() { return this->_eye; }
	inline ImVec3& direction() { return this->_direction; }
	inline ImVec3& rotation() { return this->_rotation; }
	inline ImVec3& up() { return this->_up; }
	inline float& zFocus() { return this->_zFocus; }

	inline Eigen::Matrix4f& view() { return this->_view; }
	inline Eigen::Matrix4f& projection() { return this->_projection; }
	inline Eigen::Matrix4f viewProjecion() { return this->_vp; }
	inline bool& dirtiness() { return this->_isMouseDirty; }

	inline void resetPosition() {
		this->_viewport = ImVec2(800.f, 800.f); // maybe we'll need it someday
		//this->_eye = ImVec3(0.f, 0.f, 150.f);
		this->_eye = ImVec3(0.f, 0.f, 1.f);
		this->_direction = ImVec3(0.f, 0.f, -1.f);
		this->_rotation = ImVec3(0.f, 0.f, 0.f);
		this->_up = ImVec3(0.f, 1.f, 0.f);
		this->_view = Linal::lookAt(this->_eye, this->_eye + this->_direction, this->_up);
		this->_projection = Affine::identity();
		this->_scale = Affine::identity();
		this->_zFocus = 1000.f;
		this->_isMouseDirty = false;
	}

	inline void setViewMatrix(const ImVec3& target, const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
		this->_view = Linal::lookAt(this->_eye, target, up);
	}

	// PROJECTIONS
	// PERSPECTIVE

	/* idk what to use it for for */
	inline void setPerspectiveFoVProjection(const float& FoV, const float& ratio, const float& zNear, const float& zFar) {
		this->_projection = Linal::perspectiveFoV(FoV, ratio, zNear, zFar);
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

	inline void resetProjection(const float& zFocus) {
		this->_projection = Affine::identity();
	}

	inline const float& getZFocus(const float& zFocus) {
		return this->_projection(3, 2);
	}

	inline void lookAt(const ImVec3& target) {
		setViewMatrix(target);
		setPerspectiveZFocus(this->_zFocus); //EXAMPLES!
		//setPerspectiveFoVProjection(45.f, this->_viewport.x / this->_viewport.y, 0.1f, 200.f);
		this->_vp = (this->_scale)*(this->_projection) * (this->_view);
	}
};

#endif
