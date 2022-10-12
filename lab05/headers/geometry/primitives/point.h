#pragma once
#ifndef _POINT_H_
#define _POINT_H_

#include "pch.h"
#include "geometry/primitives/primitive.h"

class Point : public Primitive
{
public:
	Point(const ImVec2& p, const ImU32& color, const float& thickness);

	void draw(ImDrawList*, const ImVec2&);
};

#endif
