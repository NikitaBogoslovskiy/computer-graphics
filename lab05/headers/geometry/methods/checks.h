#ifndef _CHECKS_H_
#define _CHECKS_H_

#include "geometry/primitives2d/point.h"
#include "geometry/primitives2d/primitive.h"
#include "geometry/primitives2d/edge.h"
#include <set>

void pointPositionWithEdge(Point& p, Edge& e, bool& onTheLeft);
void pointPositionWithConvexPolygon(Point& point, Primitive& polygon, bool& isInside);

bool intersected(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImVec2* out);
ImVector<ImVec2*> get_intersections(std::set<Primitive*> prims);

void pointPositionWithPolygon(Point& point, Primitive& polygon, bool& isInside, float canvas_width);
bool checkPointAndEdgeConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Edge*& edge);
bool checkPointAndPolygonConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Primitive*& polygon);
ImVector<ImVec2*> get_intersections(Primitive* curr, std::vector<Primitive*> prims);

#endif
