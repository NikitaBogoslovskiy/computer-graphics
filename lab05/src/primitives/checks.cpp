#include "geometry/checks.h"

void pointPositionWithEdge(Point& p, Edge& e, bool& onTheLeft)
{
	float t = e.p1().y;
	t = e.p2().y;
	float x2 = e.p2().x - e.p1().x;
	float y2 = -(e.p2().y - e.p1().y);
	float px = p.front().x - e.p1().x;
	float py = -(p.front().y - e.p1().y);
	float expression = py * x2 - px * y2;
	if (expression > 0)
		onTheLeft = true;
	else
		onTheLeft = false;
}

void pointPositionWithConvexPolygon(Point& point, Primitive& polygon, bool& isInside)
{
	size_t s = polygon.size();
	bool onTheLeft, wasLeft = false, wasRight = false;
	isInside = false;
	for (size_t i = 0; i < s; ++i)
	{
		auto edge = Edge(polygon[i % s], polygon[(i + 1) % s], polygon.color(), polygon.thickness());
		pointPositionWithEdge(point, edge, onTheLeft);
		if (onTheLeft)
			wasLeft = true;
		else
			wasRight = true;
	}
	if (wasLeft != wasRight)
		isInside = true;
}

