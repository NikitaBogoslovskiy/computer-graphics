#include "geometry/methods/checks.h"
#include "geometry/methods/funcs.h"
#include <set>

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

void pointPositionWithPolygon(Point& point, Primitive& polygon, bool& isInside, float canvas_width)
{
	auto edge = Edge(point[0], ImVec2(canvas_width, point[0].y), point.color(), point.thickness());
	std::set<Primitive*> prims = { &polygon, &edge };
	auto result = get_intersections(prims);
	int correction = 0;
	size_t polygon_size = polygon.size();
	for (size_t i = 0; i < polygon_size; ++i)
	{
		if (polygon[i].y == point[0].y && polygon[i].x > point[0].x) //we don`t count below edges
		{
			if (polygon[(i + polygon_size - 1) % polygon_size].y > polygon[i].y)
				++correction;
			if (polygon[(i + 1) % polygon_size].y > polygon[i].y)
				++correction;
		}
	}
	isInside = (result.Size - correction) % 2 == 1;
}

bool checkPointAndEdgeConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Edge*& edge)
{
	if (primitives.size() != 2)
	{
		answer = "You should choose 2 primitives";
		return false;
	}
	else
	{
		for (auto it = primitives.begin(); it != primitives.end(); ++it)
		{
			auto p = dynamic_cast<Point*>(*it);
			auto e = dynamic_cast<Edge*>(*it);
			if (p != nullptr)
			{
				if (point == nullptr)
					point = p;
				else
				{
					answer = "You should choose only one point";
					return false;
				}
			}
			else if (e != nullptr)
			{
				if (edge == nullptr)
					edge = e;
				else
				{
					answer = "You should choose only one edge";
					return false;
				}
			}
			else
			{
				answer = "You can choose only edge and point";
				return false;
			}
		}
	}
	return true;
}

bool checkPointAndPolygonConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Primitive*& polygon)
{
	if (primitives.size() != 2)
	{
		answer = "You should choose 2 primitives";
		return false;
	}
	else
	{
		for (auto it = primitives.begin(); it != primitives.end(); ++it)
		{
			if ((*it)->size() == 1)
			{
				if (point == nullptr)
					point = dynamic_cast<Point*>(*it);
				else
				{
					answer = "You should choose only one point";
					return false;
				}
			}
			else if ((*it)->size() > 2)
			{
				if (polygon == nullptr)
					polygon = *it;
				else
				{
					answer = "You should choose only one polygon";
					return false;
				}
			}
			else
			{
				answer = "You can choose only point and polygon";
				return false;
			}
		}
	}
	return true;
}

bool intersected(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImVec2* out) {
	ImVec2 n = (d - c);
	n = ImVec2(-n.y, n.x);
	float buf = (n * (b - a));
	if (!buf) return false;
	float t = -(n * (a - c)) / buf;
	*out = a + (t * (b - a));
	if ((a.x < b.x && a.x <= out->x && b.x >= out->x || a.x > b.x && b.x <= out->x && a.x >= out->x || a.y < b.y && a.y <= out->y && b.y >= out->y || a.y > b.y && b.y <= out->y && a.y >= out->y) &&
		(c.x < d.x && c.x <= out->x && d.x >= out->x || c.x > d.x && d.x <= out->x && c.x >= out->x || c.y < d.y && c.y <= out->y && d.y >= out->y || c.y > d.y && d.y <= out->y && c.y >= out->y)) return true;
	return false;
}

ImVector<ImVec2*> get_intersections(std::set<Primitive*> prims) {
	ImVector<ImVec2*> points = ImVector<ImVec2*>();
	ImVec2 out = ImVec2();
	for (auto src = prims.cbegin(); src != prims.cend(); src++) {
		auto src1 = src;
		for (auto dest = ++src1; dest != prims.cend(); dest++) {
			if (src != dest) {
				auto src_size = (*src)->size();
				auto dest_size = (*dest)->size();
				for (size_t j = 0; j < src_size - (src_size == 2 ? 1 : 0); j++) {
					for (size_t i = 0; i < dest_size - (dest_size == 2 ? 1 : 0); i++) {
						if (intersected((*src)->at(j % src_size), (*src)->at((j + 1) % src_size), (*dest)->at(i % dest_size), (*dest)->at((i + 1) % dest_size), &out)) {
							points.push_back(new ImVec2(out));
						}
					}
				}
			}
		}
	}
	return points;
}

ImVector<ImVec2*> get_intersections(Primitive* curr, std::vector<Primitive*> prims) {
	ImVector<ImVec2*> points = ImVector<ImVec2*>();
	ImVec2 out = ImVec2();
	auto curr_size = curr->size();
	for (auto dest = prims.cbegin(); dest != prims.cend(); dest++) {
		if (curr != *dest) {
			auto dest_size = (*dest)->size();
			for (size_t j = 0; j < curr_size - 1; j++) {
				for (size_t i = 0; i < dest_size - (dest_size == 2 ? 1 : 0); i++) {
					if (intersected(curr->at(j % curr_size), curr->at((j + 1) % curr_size), (*dest)->at(i % dest_size), (*dest)->at((i + 1) % dest_size), &out)) {
						points.push_back(new ImVec2(out));
					}
				}
			}
		}
	}
	return points;
}
