#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "pch.h"
#include "affine.h"

class Camera
{

public:

	/**
		eye - position of the camera
		target - the point camera targeted on
		up - up direction
	*/
	static const Eigen::Matrix4f lookAtRH(const ImVec3& eye = ImVec3(0.f, 0.f, 0.f), const ImVec3& target = ImVec3(0.f, 0.f, 0.f), const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
		ImVec3 zaxis = Affine::normalize(eye - target);		// "ray" from the eye to the target
		ImVec3 xaxis = Affine::normalize(Affine::cross(up, zaxis)); // x axis of the new coordinate system
		ImVec3 yaxis = Affine::cross(zaxis, xaxis); // y axis of the new coordinate system

		return Eigen::Matrix4f{
			{  xaxis.x,					 yaxis.x,				   zaxis.x,					 0.f },
			{  xaxis.y,					 yaxis.y,				   zaxis.y,					 0.f },
			{  xaxis.z,					 yaxis.z,				   zaxis.z,					 0.f },
			{ -Affine::dot(xaxis, eye), -Affine::dot(yaxis, eye), -Affine::dot(zaxis, eye),  1.f }
		}.transpose();
	}

	/**
		FoV - field of view angle (rad)
		ratio - screen aspect ratio (width / height)
		znear - location of the near Z clipping plane
		zfar - location of the far Z clipping plane
	*/
	static const Eigen::Matrix4f perspective(const float& FoV, const float& ratio, const float& zNear, const float& zFar) {
		float zRange = zNear - zFar;
		float tanHalfVoV = tan(FoV / 2);

		return Eigen::Matrix4f{
			{ 1.f / (tanHalfVoV * ratio), 0.f,				0.f,					  0.f						  },
			{ 0.f,						  1.f / tanHalfVoV, 0.f,					  0.f						  },
			{ 0.f,						  0.f,				(-zNear - zFar) / zRange, 2.f * zFar * zNear / zRange },
			{ 0.f,						  0.f,				1.f,					  0.f						  }
		}.transpose();
	}
};

#endif
