#ifndef _AFFINE_H_
#define _AFFINE_H

#include "../src/imgui/imgui.h"
#include "../src/imgui/imgui_impl_glfw.h"
#include "../src/imgui/imgui_impl_opengl3.h"
#include "../src/Eigen/Dense"

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
};

#endif
