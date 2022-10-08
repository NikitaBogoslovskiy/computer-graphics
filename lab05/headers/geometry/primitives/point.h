#ifndef _POINT_H_
#define _POINT_H

#include "pch.h"

class Point : public Primitive
{
public:
	Point(const ImVec2& p, const ImU32& color, const float& thickness);

	void draw(ImDrawList*, const ImVec2&);
};

#endif
