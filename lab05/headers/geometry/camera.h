#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "pch.h"
#include "linal.h"

class Camera
{
	ImVec2 _viewport; /*width, height of screen*/
	ImVec3 _eye;
	ImVec3 _rotation;
	Eigen::Matrix4f _view;
	Eigen::Matrix4f _projection;
	Eigen::Matrix4f _vp;
	ImDrawList* _draw_list;
public:
	Camera() {}

	Camera(const ImVec2& viewport, const ImVec3& eye, ImDrawList*& draw_list) {
		this->_viewport = viewport;
		this->_eye = eye;
		this->_draw_list = draw_list;
	}

	inline ImVec2& getViewport() { return this->_viewport; }
	inline ImVec3& getEye() { return this->_eye; }
	inline ImVec3& getRotation() { return this->_rotation; }

	inline Eigen::Matrix4f& getView() { return this->_view; }
	inline Eigen::Matrix4f& getProjection() { return this->_projection; }
	inline Eigen::Matrix4f getViewProjecion() { return this->_vp; }

	inline void setViewport(const ImVec2& viewport) { this->_viewport = viewport; }
	inline void setEye(const ImVec3& eye) { this->_eye = eye; }
	inline void setRotation(const ImVec3& rotation) { this->_rotation = rotation; }

	inline void setViewMatrix(const ImVec3& target, const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) { 
		this->_view = Linal::lookAt(this->_eye, target, up); 
	}
	inline void setPerspectiveProjection(const float& FoV, const float& ratio, const float& zNear, const float& zFar) { 
		this->_projection = Linal::perspective(FoV, ratio, zNear, zFar); 
	}
	inline void setAxonometryProjection(const float& angleX, const float& angleY) { 
		this->_projection = Linal::axonometry(angleX, angleY); 
	}
	
	inline void lookAt(const ImVec3& target) {
		setViewMatrix(target);
		//setPerspectiveProjection(45.f, _viewport.x / _viewport.y, 0.1f, 1000.f); //EXAMPLES!
		setPerspectiveProjection(45.f, 3620.f / 240.f, 0.1f, 1000.f); //EXAMPLES!
		this->_vp = (this->_projection) * (this->_view);
	}
};

#endif
