#ifndef _POINT_H_
#define _POINT_H

#include "../../../src/imgui/imgui.h"
#include "../../../src/imgui/imgui_impl_glfw.h"
#include "../../../src/imgui/imgui_impl_opengl3.h"
#include "../../../src/Eigen/Dense"

class Point : public Primitive
{
public:
	Point(const ImVec2& p, const ImU32& color, const float& thickness);

	void draw(ImDrawList*, const ImVec2&);
};

#endif
