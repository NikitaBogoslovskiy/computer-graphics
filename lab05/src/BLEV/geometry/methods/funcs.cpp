#include "geometry/methods/funcs.h"
#include "geometry/methods/linal.h"
#include "geometry/primitives2d/point.h"
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

Primitive* packPresent(Primitive* current_point, const std::set<Primitive*>& chosen_prims, const ImU32& color, const float& thickness) {
	auto res = new Primitive(color, thickness);
	res->push_back(current_point->front());

	ImVec2 prev_point = ImVec2(current_point->front().x - 10, current_point->front().y); // just any straight horizontal line

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