#ifndef _CHECKS_H_
#define _CHECKS_H_

#include "geometry/primitives/point.h"
#include "geometry/primitives/primitive.h"
#include "geometry/primitives/edge.h"

void pointPositionWithEdge(Point& p, Edge& e, bool& onTheLeft);
void pointPositionWithConvexPolygon(Point& point, Primitive& polygon, bool& isInside);

#endif
