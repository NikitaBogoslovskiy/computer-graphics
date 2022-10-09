#ifndef _BEZIER_CURVE_H_
#define _BEZIER_CURVE_H_

#include "pch.h"

class BezierCurve : public Primitive
{
	ImVec2& nextBt3(const ImVector<ImVec2>* points, const float& t);
public:
	//BezierCurve(ImVector<ImVec2>* points, const ImU32& color, const float& thickness);
	BezierCurve(const ImU32& color, const float& thickness);
	void draw(ImDrawList* draw_list, const ImVec2& offset);
	void draw_skeleton(ImDrawList* draw_list, const ImVec2& offset);
	void draw_previe(ImDrawList* draw_list, const ImVec2& offset);
	//void BezierCurve::draw(ImDrawList* draw_list, const ImVec2& offset, bool connect_bounds)
};

#endif