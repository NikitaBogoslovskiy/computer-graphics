#ifndef _BEZIER_CURVE_H_
#define _BEZIER_CURVE_H_
#include "pch.h"
#include <deque>

class BezierCurve : public Primitive
{
	float _step = 0.02;
	void draw_skeleton(ImDrawList* draw_list, const ImVec2& offset);
	ImVec2& nextBt3(const std::deque<ImVec2>& points, const float& t);
	void draw_curve3(ImDrawList* draw_list, const std::deque<ImVec2>& points, const ImVec2& offset);
public:
	//BezierCurve(ImVector<ImVec2>* points, const ImU32& color, const float& thickness);
	BezierCurve(const ImU32& color, const float& thickness);
	void draw(ImDrawList* draw_list, const ImVec2& offset);
	void draw_previe(ImDrawList* draw_list, const ImVec2& offset);
	//void BezierCurve::draw(ImDrawList* draw_list, const ImVec2& offset, bool connect_bounds)
};

#endif