#include "geometry/methods/funcs.h"
#include "geometry/primitives2d/point.h"
#include "geometry/primitives2d/edge.h"
#include "geometry/methods/linal.h"
#include "geometry/methods/checks.h"
#include <set>

Primitive midpointDisplacement(Primitive& displacement, Point* p1, Point* p2, int R, int I, int iter_num)
{
	auto color = p1->color();
	float thikness = 1;
	Primitive result(color, thikness);
	std::vector<ImVec2> points1;
	if (displacement.size() == 0)
	{
		points1.push_back(p1->at(0));
		points1.push_back(p2->at(0));
		if (iter_num == 1)
		{
			result.push_back(points1[0]);
			result.push_back(points1[1]);
			return result;
		}
		--iter_num;
	}
	else
	{
		for (size_t i = 0; i < displacement.size(); ++i)
			points1.push_back(displacement[i]);
	}

	std::vector<ImVec2> points2;
	std::vector<ImVec2>::iterator start_it = points1.begin(), end_it = points1.end();
	std::vector<ImVec2>* current_vec = &points2;
	for (size_t i = 0; i < iter_num; ++i)
	{
		auto it = start_it;
		while (it != end_it - 1)
		{
			auto next = it + 1;
			float x = (it->x + next->x) / 2;
			float y = (it->y + next->y) / 2 + (R == 0 || I == 0 ? 0 : rand() % (2 * R * I) - R * I);
			current_vec->push_back(*it);
			current_vec->push_back(ImVec2(x, y));
			it = next;
		}
		current_vec->push_back(*it);

		start_it = current_vec->begin();
		end_it = current_vec->end();
		if (i % 2 == 0)
		{
			points1.clear();
			current_vec = &points1;
		}
		else
		{
			points2.clear();
			current_vec = &points2;
		}
	}

	for (auto it = start_it; it != end_it; ++it)
		result.push_back(*it);

	return result;
}

Primitive* packPresent(Point* bottom_point, const std::set<Primitive*>& chosen_prims, const ImU32& color, const float& thickness) {
	auto res = new Primitive(color, thickness);
	Primitive* current_point = bottom_point;
	ImVec2 prev_point = ImVec2(current_point->front().x - 10, current_point->front().y); // just any straight horizontal line

	res->push_back(current_point->front());

	Primitive* first_point = current_point;
	while (true) {
		float min_cos = 1;
		float min_distance = std::numeric_limits<float>::max();

		auto v1 = prev_point - current_point->front();
		Primitive* next_point = nullptr;
		for (auto it = chosen_prims.begin(); it != chosen_prims.end(); ++it)
		{
			Primitive* p = *it;
			if (p == current_point) continue;
			auto v2 = p->front() - current_point->front();
			auto distance = Linal::len(v2);

			auto cos = v1 * v2 / (Linal::len(v1) * distance);

			if ((cos < min_cos) || (cos == min_cos) && (distance < min_distance)) {
				min_cos = cos;
				next_point = p;
				min_distance = distance;
			}
		}
		if (next_point == first_point) break;
		prev_point = current_point->front();
		current_point = next_point;
		res->push_back(current_point->front());
	}

	return res;
}

static float dist(ImVec2& p1, ImVec2& p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

ImVector<ImVec2>* unionPolygons(Primitive* p1, Primitive* p2)
{
	auto edge = Edge(p1->at(0), p1->at(1), p1->color(), p1->thickness());
	auto point = Point(p1->center(), p1->color(), p1->thickness());
	bool onTheLeft;
	pointPositionWithEdge(point, edge, onTheLeft);
	if (!onTheLeft)
		p1->reverse();
	edge = Edge(p2->at(0), p2->at(1), p2->color(), p2->thickness());
	point = Point(p2->center(), p2->color(), p2->thickness());
	pointPositionWithEdge(point, edge, onTheLeft);
	if (!onTheLeft)
		p2->reverse();

	ImVector<ImVec2>* result = new ImVector<ImVec2>();
	std::unordered_map<int, std::vector<std::tuple<ImVec2, int>>>* currentMap;
	std::unordered_map<int, std::vector<std::tuple<ImVec2, int>>> p1_to_p2;
	std::unordered_map<int, std::vector<std::tuple<ImVec2, int>>> p2_to_p1;
	bool now_p1;
	float leftest_x = FLT_MAX;
	int leftest_point_index = -1;
	int p1_size = p1->size(), p2_size = p2->size();
	ImVec2 out = ImVec2();
	for (size_t i = 0; i < p1_size; i++) {
		if (p1->at(i).x < leftest_x)
		{
			now_p1 = true;
			leftest_x = p1->at(i).x;
			leftest_point_index = i;
		}
		for (size_t j = 0; j < p2_size; j++) {
			if (p2->at(i).x < leftest_x)
			{
				now_p1 = false;
				leftest_x = p2->at(i).x;
				leftest_point_index = j;
			}
			if (intersected(p1->at(i), p1->at((i + 1) % p1_size), p2->at(j), p2->at((j + 1) % p2_size), &out)) {
				p1_to_p2[(i + 1) % p1_size].push_back(std::make_tuple(out, (j + 1) % p2_size));
				p2_to_p1[(j + 1) % p2_size].push_back(std::make_tuple(out, (i + 1) % p1_size));
			}
		}
	}

	if (p1_to_p2.size() == 0)
	{
		bool isInside;
		if (now_p1)
		{
			Point p = Point(p2->at(0), p2->color(), p2->thickness());
			pointPositionWithConvexPolygon(p, *p1, isInside);
			if (isInside)
				return new ImVector<ImVec2>(*(p1->getPoints()));
			else
				return nullptr;
		}
		else
		{
			Point p = Point(p1->at(0), p1->color(), p1->thickness());
			pointPositionWithConvexPolygon(p, *p2, isInside);
			if (isInside)
				return new ImVector<ImVec2>(*(p2->getPoints()));
			else
				return nullptr;
		}
	}

	Primitive* currentPrimitive, * finalPrimitive;
	int currentSize;
	if (now_p1)
	{
		currentPrimitive = p1;
		currentMap = &p1_to_p2;
		currentSize = p1_size;
	}
	else
	{
		currentPrimitive = p2;
		currentMap = &p2_to_p1;
		currentSize = p2_size;
	}
	finalPrimitive = currentPrimitive;
	int currentIndex = (leftest_point_index + 1) % currentSize;
	result->push_back(currentPrimitive->at(leftest_point_index));
	while (true)
	{
		auto it = currentMap->find(currentIndex);
		if (it == currentMap->end())
		{
			result->push_back(currentPrimitive->at(currentIndex));
			if (currentPrimitive == finalPrimitive && currentIndex == leftest_point_index)
				break;
			currentIndex = (currentIndex + 1) % currentSize;
		}
		else
		{
			float minDist = FLT_MAX;
			int chosenIndex;
			for (size_t k = 0; k < it->second.size(); ++k)
			{
				float d = dist(std::get<0>(it->second[k]), currentPrimitive->at((currentIndex - 1) % currentSize));
				if (d < minDist)
				{
					minDist = d;
					chosenIndex = k;
				}
			}
			result->push_back(std::get<0>(it->second[chosenIndex]));
			currentIndex = std::get<1>(it->second[chosenIndex]);
			if (now_p1)
			{
				currentPrimitive = p2;
				currentMap = &p2_to_p1;
				currentSize = p2->size();
				now_p1 = false;
			}
			else
			{
				currentPrimitive = p1;
				currentMap = &p1_to_p2;
				currentSize = p1->size();
				now_p1 = true;
			}
			result->push_back(currentPrimitive->at(currentIndex));
			if (currentPrimitive == finalPrimitive && currentIndex == leftest_point_index)
				break;
			currentIndex = (currentIndex + 1) % currentSize;
		}
	}
	return result;
}
