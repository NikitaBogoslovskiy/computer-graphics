#ifndef _EDGE_H_
#define _EDGE_H

#include "../../../src/imgui/imgui.h"
#include "../../../src/imgui/imgui_impl_glfw.h"
#include "../../../src/imgui/imgui_impl_opengl3.h"
#include "../../../src/Eigen/Dense"

class Edge : public Primitive
{
public:
	Edge(const ImVec2& p1, const ImVec2& p2, const ImU32& color, const float& thickness);
	inline ImVec2& p1() { return front(); }
	inline ImVec2& p2() { return back(); }

	void rotate(const float& angle);
};

#endif
