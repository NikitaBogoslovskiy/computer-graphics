#ifndef _FUNCS_H_
#define _FUNCS_H

#include "../../src/imgui/imgui.h"
#include "../../src/imgui/imgui_impl_glfw.h"
#include "../../src/imgui/imgui_impl_opengl3.h"
#include "../../src/Eigen/Dense"

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline float operator*(const ImVec2& lhs, const ImVec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
inline ImVec2 operator*(const float& lhs, const ImVec2& rhs) { return ImVec2(lhs * rhs.x, lhs * rhs.y); }

#endif
