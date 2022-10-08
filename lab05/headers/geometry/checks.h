#ifndef _CHECKS_H_
#define _CHECKS_H_

void pointPositionWithEdge(Point& p, Edge& e, bool& onTheLeft);
void pointPositionWithConvexPolygon(Point& point, Primitive& polygon, bool& isInside);

#endif
