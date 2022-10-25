#include "geometry/primitives2d/point.h"
#include "geometry/methods/funcs.h"

Point::Point(const ImVec2& p, const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{
	push_back(p);
}

void Point::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	if (show()) {
		draw_list->AddCircleFilled(this->operator[](0) + offset, thickness(), color(), 10);
	}
}