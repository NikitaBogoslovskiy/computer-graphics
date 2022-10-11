#ifndef _AFFINE_H_
#define _AFFINE_H_

#include "pch.h"

class Affine
{
public:
	static const Eigen::Matrix3f& rotate(const float& angle, const ImVec2& d = ImVec2(0.f, 0.f)) {
		float cosa = cosf(angle);
		float sina = sinf(angle);
		Eigen::Matrix3f ret{
			{ cosa, sina, 0 },
			{ -sina, cosa, 0 },
			{ -d.x * cosa + d.y * sina + d.x, -d.x * sina - d.y * cosa + d.y, 1.f}
		};
		return ret;
	}

	static const Eigen::Matrix3f& scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2& d = ImVec2(0.f, 0.f)) {
		Eigen::Matrix3f ret{
			{ scaleCoeffX, 0.f, 0.f },
			{ 0.f, scaleCoeffY, 0.f },
			{ (1.f - scaleCoeffX) * d.x, (1.f - scaleCoeffY) * d.y, 1.f}
		};
		return ret;
	}

	static const Eigen::Matrix3f& translate(const ImVec2& d = ImVec2(0.f, 0.f)) {
		Eigen::Matrix3f ret{
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ -d.x, - d.y, 1.f}
		};
		return ret;
	}
};

#endif
