#pragma warning( disable : 4244 )
#ifndef _FUNCS_H_
#define _FUNCS_H_

#include "pch.h"

#define PI 3.14159265359f

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline float operator*(const ImVec2& lhs, const ImVec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
inline ImVec2 operator*(const float& lhs, const ImVec2& rhs) { return ImVec2(lhs * rhs.x, lhs * rhs.y); }
inline bool operator==(const ImVec2& lhs, const ImVec2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
inline float operator*(const ImVec4& lhs, const ImVec4& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }
inline ImVec4 operator*(const float& lhs, const ImVec4& rhs) { return ImVec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }


inline ImU32 GetColorV4U32(const ImVec4& curr_color) {
	return IM_COL32(curr_color.x, curr_color.y, curr_color.z, curr_color.w);
}

inline ImU32 GetColorFlU32(const float* curr_color) {
	return (IM_COL32((int)(curr_color[0] * 255), (int)(curr_color[1] * 255), (int)(curr_color[2] * 255), (int)(curr_color[3] * 255)));
}

inline ImVec4 GetColorFlV4(const float* curr_color) {
	return ImVec4((int)(curr_color[0] * 255), (int)(curr_color[1] * 255),(int)(curr_color[2] * 255), (int)(curr_color[3] * 255));
}

inline void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

inline void HelpPrevItem(const char* desc)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

inline float DegreesToRadians(const float& degrees) {
	return degrees * (2 * acos(0.0) / 180);
}

inline bool intersected(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImVec2* out) {
	ImVec2 n = (d - c);
	n = ImVec2(-n.y, n.x);
	float buf = (n * (b - a));
	if (!buf) return false;
	float t = -(n * (a - c)) / buf;
	*out = a + (t * (b - a));
	if ((a.x < b.x && a.x <= out->x && b.x >= out->x || a.x > b.x && b.x <= out->x && a.x >= out->x || a.y < b.y && a.y <= out->y && b.y >= out->y || a.y > b.y && b.y <= out->y && a.y >= out->y) &&
		(c.x < d.x && c.x <= out->x && d.x >= out->x || c.x > d.x && d.x <= out->x && c.x >= out->x || c.y < d.y && c.y <= out->y && d.y >= out->y || c.y > d.y && d.y <= out->y && c.y >= out->y)) return true;
	return false;
}

#endif
