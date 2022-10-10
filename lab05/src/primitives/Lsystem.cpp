#include "geometry.h"
#include <set>
#include <time.h>

void Lsystem::calculate_fractal(const ImU32& color, const float& thickness)
{
	_fractal = new Primitive(color, thickness);
	_fractal->push_back(ImVec2(0.f, 0.f));
	float curr_angle = 0.f;
	for (char c : _axiom) {
		switch (c)
		{
		case '+':
			curr_angle += _angle;
			break;
		case '-':
			curr_angle -= _angle;
			break;
		case '[':
			// save
			break;
		case ']':
			// restore
			break;
		default:
			if (_iters == 0) {
				calculate_edge(curr_angle);
			}
			else {
				auto it = std::find_if(_rules.cbegin(), _rules.cend(), [=](std::pair<char, std::string> p) { return p.first == c; });
				calculate_rule(&curr_angle, (*it).second, 1);
			}
			break;
		}
	}
}

void Lsystem::calculate_rule(float* curr_angle, const std::string& pattern, size_t iter)
{
	if (iter > _iters) {
		calculate_edge(*curr_angle);
		return;
	}
	for (const auto& c : pattern) {
		switch (c)
		{
		case '+':
			*curr_angle += _random_angle ? _angle * (rand() % 101) / 100.f : _angle;
			break;
		case '-':
			*curr_angle -= _random_angle ? _angle * (rand() % 101) / 100.f : _angle;
			break;
		case '[':
			// save
			break;
		case ']':
			// restore & set random off
			_random_angle = false;
			break;
		case '@':
			// set random on
			srand((unsigned int)time(0)/2);
			_random_angle = true;
			break;
		default:
			if (iter == _iters) {
				calculate_edge(*curr_angle);
			}
			else {
				auto it = std::find_if(_rules.cbegin(), _rules.cend(), [=](std::pair<char, std::string> p) { return p.first == c; });
				calculate_rule(curr_angle, (*it).second, iter + 1);
			}
			break;
		}
	}
}

void Lsystem::calculate_edge(const float& curr_angle)
{
	_fractal->push_back(_fractal->back() + _line_length * ImVec2(cosf(curr_angle), sinf(curr_angle)));
}

void Lsystem::draw(ImDrawList* draw_list, const ImVec2& offset) //, const ImU32& color, const float& thickness
{
	//_fractal->color() = color;
	//_fractal->thickness() = thickness;
	_fractal->draw_polyline(draw_list, offset);
}

bool Lsystem::check_Lsystem(const std::string& axiom, const std::vector<std::pair<char, std::string>>& rules)
{
	std::set<char> terms{};

	for (auto p : rules)
		terms.insert(p.first);

	terms.insert('+');
	terms.insert('-');
	terms.insert('[');
	terms.insert(']');
	terms.insert('@');

	if (terms.size() != rules.size() + 5) {
		return false;
	}

	for (char c : axiom) {
		if (terms.find(c) == terms.end()) {
			return false;
		}
	}

	for (auto rule : rules) {
		if (rule.first == '\0' || rule.second[0] == '\0') {
			continue;
		}
		for (char c : rule.second) {
			if (terms.find(c) == terms.end()) {
				return false;
			}
		}
	}

	_axiom = axiom;

	for (auto rule : rules) {
		if (rule.first == '\0' || rule.second[0] == '\0') {
			continue;
		}
		_rules.push_back(rule);
	}

	return true;
}
