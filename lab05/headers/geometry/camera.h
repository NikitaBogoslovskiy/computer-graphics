#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "pch.h"
#include "linal.h"

class Camera
{
	ImVec2 _viewport; /*width, height of screen*/
	ImVec3 _eye;
	ImVec3 _rotation;
	ImVec3 _up;
	Eigen::Matrix4f _view;
	Eigen::Matrix4f _projection;
	Eigen::Matrix4f _vp;
	ImDrawList* _draw_list;
public:
	Camera() {
		this->_projection = Affine::identity();
		this->_up = ImVec3(0.f, 1.f, 0.f);
		this->_eye = ImVec3(2.f, 3.f, 5.f);
	}

	Camera(const ImVec2& viewport, const ImVec3& eye, Eigen::Matrix4f& projection, ImDrawList*& draw_list) {
		this->_projection = projection;
		this->_viewport = viewport;
		this->_eye = eye;
		this->_draw_list = draw_list;
	}

	inline ImVec2& getViewport() { return this->_viewport; }
	inline ImVec3& getEye() { return this->_eye; }
	inline ImVec3& getRotation() { return this->_rotation; }
	inline ImVec3& getUp() { return this->_up; }

	inline Eigen::Matrix4f& getView() { return this->_view; }
	inline Eigen::Matrix4f& getProjection() { return this->_projection; }
	inline Eigen::Matrix4f getViewProjecion() { return this->_vp; }

	inline void setViewport(const ImVec2& viewport) { this->_viewport = viewport; }
	inline void setEye(const ImVec3& eye) { this->_eye = eye; }
	inline void setRotation(const ImVec3& rotation) { this->_rotation = rotation; }
	inline void setUp(const ImVec3& up) { this->_up = up; }

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
			if (this->_projection(i, i) + d < 1.f) continue;
			this->_projection(i, i) += d;
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
		setPerspectiveZFocus(200.f); //EXAMPLES!
		this->_vp = (this->_projection) * (this->_view);
	}
};

#endif
