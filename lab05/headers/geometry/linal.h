#pragma once

#include "pch.h"
#include "primitives3d/structers.h"
#include "funcs.h"
#include "affine.h"
#include <iostream>

class Linal
{
public:
	static const float len(const ImVec3& v) {
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	// "scalar multiplication"
	static const float dot(const ImVec3& lhs, const ImVec3& rhs) {
		return lhs * rhs;
	}

	// "vector multiplication"
	static const ImVec3 cross(const ImVec3& lhs, const ImVec3& rhs) {
		return  ImVec3(lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
	}

	static const ImVec3 normalize(const ImVec3& v) {
		return  v / len(v);
	}

	// VIEW MATRICES

	/**
		eye - position of the camera
		target - the point camera targeted on
		up - up direction
	*/
	static const Eigen::Matrix4f lookAt(const ImVec3& eye = ImVec3(0.f, 0.f, 0.f), const ImVec3& target = ImVec3(0.f, 0.f, 0.f), const ImVec3& up = ImVec3(0.f, 1.f, 0.f)) {
		ImVec3 zaxis = normalize(target - eye);		// "ray" from the eye to the target
		ImVec3 xaxis = normalize(cross(up, zaxis)); // x axis of the new coordinate system
		ImVec3 yaxis = cross(zaxis, xaxis); // y axis of the new coordinate system

		return Eigen::Matrix4f{
			{  xaxis.x,	xaxis.y, xaxis.z, -dot(xaxis, eye) },
			{  yaxis.x,	yaxis.y, yaxis.z, -dot(yaxis, eye) },
			{  zaxis.x,	zaxis.y, zaxis.z, -dot(zaxis, eye) },
			{  0.f,		0.f,	 0.f,	   1.f			   }
		}; // = orientation * translation
	}

	// PROJECTIONS

	/* unneccessary blood and sweat */
	static const Eigen::Matrix4f perspective(const float& r) {
		return Eigen::Matrix4f{
			{ 1.f, 0.f,  0.f,     0.f },
			{ 0.f, 1.f,  0.f,     0.f },
			{ 0.f, 0.f,	 0.f,     0.f },
			{ 0.f, 0.f, -1.f / r, 1.f }
		};
	}

	static const Eigen::Matrix4f perspectiveFoV(const float& FoV, const float& ratio, const float& zNear, const float& zFar) {
		float zRange = zFar - zNear;
		float tanHalfVoV = tan(DegreesToRadians(FoV) / 2);

		return Eigen::Matrix4f{
			{ 1.f / (tanHalfVoV * ratio), 0.f,				 0.f,					   0.f						   },
			{ 0.f,						  1.f / (tanHalfVoV),  0.f,					   0.f						   },
			{ 0.f,						  0.f,			    -(zNear + zFar) / zRange, -2.f * zFar * zNear / zRange },
			{ 0.f,						  0.f,				-1.f,					   0.f						   }
		};

		/*float s = 1.f / tan(DegreesToRadians(FoV) * 0.5f * PI * 180.f);

		return Eigen::Matrix4f{
			{ s,    0.f,  0.f,			  0.f					},
			{ 0.f,	s,    0.f,			  0.f					},
			{ 0.f,	0.f, -zFar / zRange, -zFar * zNear / zRange },
			{ 0.f,	0.f,  -1.f,			  0.f					}
		};*/
	}


	/**
		angleX - angle of xAxis rotation
		angleY - angle of yAxis rotation
	*/
	static const Eigen::Matrix4f axonometry(const float& angleX, const float& angleY) {
		auto _angleX = DegreesToRadians(angleX); 
		auto _angleY = DegreesToRadians(angleY);
		float sinX = sin(_angleX); float cosX = cos(_angleX);
		float sinY = sin(_angleY); float cosY = cos(_angleY);

		return Eigen::Matrix4f{
			{ cosY,		   0.f,	  sinY,		   0.f },
			{ sinX * sinY, cosX, -sinX * cosY, 0.f },
			{ 0.f,		   0.f,	  0.f,		   0.f },
			{ 0.f,		   0.f,	  0.f,		   1.f }
		};
	}

};