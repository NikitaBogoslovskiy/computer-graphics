#ifndef _AFFINE_H_
#define _AFFINE_H_

#include "pch.h"
#include "primitives3d/structers.h"
#include "funcs.h"

class Affine
{
public:
	static const Eigen::Matrix3f rotate(const float& angle, const ImVec2& d = ImVec2(0.f, 0.f)) {
		float cosa = cosf(angle);
		float sina = sinf(angle);
		return Eigen::Matrix3f {
			{ cosa, sina, 0 },
			{ -sina, cosa, 0 },
			{ -d.x * cosa + d.y * sina + d.x, -d.x * sina - d.y * cosa + d.y, 1.f}
		};
	}

	static const Eigen::Matrix3f scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2& d = ImVec2(0.f, 0.f)) {
		return Eigen::Matrix3f {
			{ scaleCoeffX, 0.f, 0.f },
			{ 0.f, scaleCoeffY, 0.f },
			{ (1.f - scaleCoeffX) * d.x, (1.f - scaleCoeffY) * d.y, 1.f}
		};
	}

	static const Eigen::Matrix3f translate(const ImVec2& d = ImVec2(0.f, 0.f)) {
		return Eigen::Matrix3f {
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ -d.x, - d.y, 1.f}
		};
	}

	// ===> 3d
	static const float len(const ImVec3& v) {
		return v.x * v.x + v.y * v.y + v.z * v.z;
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
		return  v / Affine::len(v);
	}

	// all the methods are for multiplication onto COLUMN-vectors

	// rotation about x-axis
	static const Eigen::Matrix4f rotateX(const float& angle) {
		float cosa = cosf(angle);
		float sina = sinf(angle);
		return Eigen::Matrix4f{
			{ 1.f,  0.f,  0.f,  0.f },
			{ 0.f,  cosa, sina, 0.f },
			{ 0.f, -sina, cosa, 0.f },
			{ 0.f,  0.f,  0.f,	1.f }
		}.transpose();
	}

	// rotation about y-axis
	static const Eigen::Matrix4f rotateY(const float& angle) {
		float cosa = cosf(angle);
		float sina = sinf(angle);
		return Eigen::Matrix4f{
			{ cosa, 0.f, sina, 0.f },
			{ 0.f,  1.f,  0.f,  0.f },
			{ -sina, 0.f,  cosa, 0.f },
			{ 0.f,  0.f,  0.f,  1.f }
		}.transpose();
	}

	// rotation about z-axis
	static const Eigen::Matrix4f rotateZ(const float& angle) {
		float cosa = cosf(angle);
		float sina = sinf(angle);
		return Eigen::Matrix4f{
			{  cosa,  sina, 0.f, 0.f },
			{ -sina,  cosa, 0.f, 0.f },
			{  0.f,   0.f,  1.f, 0.f },
			{  0.f,   0.f,  0.f, 1.f }
		}.transpose();
	}

	static const Eigen::Matrix4f scale(const float& scaleCoeffX, const float& scaleCoeffY, const float& scaleCoeffZ) {
		return Eigen::Matrix4f{
			{ scaleCoeffX, 0.f,			0.f,		 0.f },
			{ 0.f,		   scaleCoeffY, 0.f,		 0.f },
			{ 0.f,		   0.f,			scaleCoeffZ, 0.f },
			{ 0.f,		   0.f,			0.f,		 1.f },
		};
	}

	// maybe we should decide if minuses are appended or they come strictly from d
	static const Eigen::Matrix4f translate(const ImVec3& d = ImVec3(0.f, 0.f, 0.f)) {
		return Eigen::Matrix4f{
			{ 1.f, 0.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ d.x, d.y, d.z, 1.f },
		}.transpose();
	}

	static const Eigen::Matrix4f identity() {
		return Eigen::Matrix4f{
			{ 1.f, 0.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 0.f, 1.f }
		};
	}

};

#endif
