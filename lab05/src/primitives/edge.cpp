#include "geometry/primitives/edge.h"
#include "geometry/affine.h"
#include <stdio.h>
#include <cstdlib>

Edge::Edge(const ImVec2& p1, const ImVec2& p2, const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{
	push_back(p1);
	push_back(p2);
}
