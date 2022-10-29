#ifndef _POINT_H_
#define _POINT_H_

#include "pch.h"
#include "primitive.h"

class Point : public Primitive
{
public:
	Point(const ImVec2& p, const ImU32& color, const float& thickness);
	inline ImVec2& get() { return points->Data[0]; }
	void draw(ImDrawList*, const ImVec2&);
};

#endif
