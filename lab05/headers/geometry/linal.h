#pragma once

#include "pch.h"
#include "primitives3d/structers.h"
#include "funcs.h"
#include "affine.h"

class Linal
{
public:

	// transposing - for multiplication onto COLUMN-vector

		// VIEW MATRICES

		/**
			eye - position of the camera
			target - the point camera targeted on
			up - up direction
		*/
	static const Eigen::Matrix4f lookAt(const ImVec3& eye = ImVec3(0.f, 0.f, 0.f), const ImVec3& target = ImVec3(0.f, 0.f, 0.f), const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
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

	// PROJECTIONS

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
		};
	}

	/**
		angleX - angle of xAxis rotation
		angleY - angle of yAxis rotation
	*/
	static const Eigen::Matrix4f axonometry(const float& angleX, const float& angleY) {
		float sinX = sin(angleX); float cosX = cos(angleX);
		float sinY = sin(angleY); float cosY = cos(angleY);

		return Eigen::Matrix4f{
			{ cosY,		   0.f,	  sinY,		   0.f },
			{ sinX * sinY, cosX, -sinX * cosY, 0.f },
			{ 0.f,		   0.f,	  0.f,		   0.f },
			{ 0.f,		   0.f,	  0.f,		   1.f }
		};
	}

};