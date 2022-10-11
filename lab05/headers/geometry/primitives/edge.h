#ifndef _EDGE_H_
#define _EDGE_H_

#include "pch.h"

class Edge : public Primitive
{
public:
	Edge(const ImVec2& p1, const ImVec2& p2, const ImU32& color, const float& thickness);
	inline ImVec2& p1() { return front(); }
	inline ImVec2& p2() { return back(); }

	void rotate(const float& angle);
};

#endif
