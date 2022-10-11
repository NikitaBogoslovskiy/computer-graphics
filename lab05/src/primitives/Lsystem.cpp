#include "geometry/primitives/Lsystem.h"
#include "geometry/affine.h"
#include <time.h>
#include <cstdlib>
#include <tuple>
#include <stack>

#define PI 3.14159265359f

void Lsystem::calculate_fractal(const ImU32& color, const float& thickness)
{
	std::stack<std::tuple<float, ImVec2, bool>> saved_states;
	_prims_iter_start.push_back(0);
	_prims.push_back(new Primitive(color, thickness));
	auto _fractal = _prims.back();

	_fractal->push_back(ImVec2(0.f, 0.f));
	float curr_angle = _is_tree ? -PI / 2.f : 0.f;
	for (char c : _axiom) {
		switch (c)
		{
		case '+':
			curr_angle += (_random_angle ? _angle * ((rand() % 100) / 100.f) : _angle);
			break;
		case '-':
			curr_angle -= (_random_angle ? _angle * ((rand() % 100) / 100.f) : _angle);
			break;
		case '[':
			// save
			saved_states.push(std::make_tuple(curr_angle, _fractal->back(), _random_angle));
			break;
		case ']':
			// restore
			curr_angle = saved_states.top()._Myfirst._Val;
			_prims_iter_start.push_back(0);
			_prims.push_back(new Primitive(color, _thickness));
			_fractal = _prims.back();
			_fractal->push_back(saved_states.top()._Get_rest()._Myfirst._Val);
			_random_angle = saved_states.top()._Get_rest()._Get_rest()._Myfirst._Val;
			saved_states.pop();
			break;
		case '@':
			// set random on
			_random_angle = true;
			break;
		default:
			if (_iters == 0 || _fwds.find(c) != _fwds.end()) {
				calculate_edge(curr_angle, 0);
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
	std::stack<std::tuple<float, ImVec2, bool>> saved_states;
	auto _fractal = _prims.back();

	if (iter == _iters) {
		calculate_edge(*curr_angle, iter);
		return;
	}
	for (const auto& c : pattern) {
		switch (c)
		{
		case '+':
			*curr_angle += (_random_angle ? _angle * ((rand() % 100) / 100.f) : _angle);
			break;
		case '-':
			*curr_angle -= (_random_angle ? _angle * ((rand() % 100) / 100.f) : _angle);
			break;
		case '[':
			// save
			saved_states.push(std::make_tuple(*curr_angle, _fractal->back(), _random_angle));
			break;
		case ']':
			// restore
			*curr_angle = saved_states.top()._Myfirst._Val;
			_prims_iter_start.push_back(iter);
			_prims.push_back(new Primitive(GetColorV4U32(_src_color), _thickness));
			_fractal = _prims.back();
			_fractal->push_back(saved_states.top()._Get_rest()._Myfirst._Val);
			_random_angle = saved_states.top()._Get_rest()._Get_rest()._Myfirst._Val;
			saved_states.pop();
			break;
		case '@':
			// set random on
			_random_angle = true;
			break;
		default:
			if (_iters == 0 || (_fwds.find(c) != _fwds.end())) {
				calculate_edge(*curr_angle, iter);
			}
			else {
				auto it = std::find_if(_rules.cbegin(), _rules.cend(), [=](std::pair<char, std::string> p) { return p.first == c; });
				calculate_rule(curr_angle, (*it).second, iter + 1);
			}
			break;
		}
	}
}

void Lsystem::calculate_edge(const float& curr_angle, const size_t& iter)
{
	_prims.back()->push_back(_prims.back()->back() + (_is_tree ? ((_iters - iter + 1.f) / _iters * _line_length) : _line_length) * ImVec2(cosf(curr_angle), sinf(curr_angle)));
}

float max(const float& a, const float& b) {
	return a > b ? a : b;
}

void Lsystem::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	if (_show) {
		if (_is_tree) {
			if (_is_alive) {
				_thickness = max(sinf(ImGui::GetTime()) * 7, 1.f);
			}
			ImVec4& src = _src_color;
			ImVec4& dest = _dest_color;
			for (size_t i = 0; i < _prims.size(); i++) {
				ImVec4 col_offset = (1.f / _iters) * (dest - src);
				float th_offset = (1.f - _thickness) / _iters;
				_prims[i]->draw_polyline(draw_list, offset, src + (_prims_iter_start[i] * col_offset), col_offset, _thickness + (_prims_iter_start[i] * th_offset), th_offset);
			}
		}
		else {
			for (auto _fractal : _prims) {
				_fractal->draw_polyline(draw_list, offset);
			}
		}
	}
}

bool Lsystem::check_Lsystem(const std::vector<std::pair<char, std::string>>& rules, const std::string& additional)
{
	std::set<char> terms{};

	for (auto p : rules) {
		terms.insert(p.first);
	}

	terms.insert('+');
	terms.insert('-');
	terms.insert('[');
	terms.insert(']');
	terms.insert('@');

	if (terms.size() != rules.size() + 5) {
		return false;
	}

	_fwds = std::set<char>();
	for (char c : additional) {
		if (terms.find(c) != terms.end()) {
			return false;
		}
		terms.insert(c);
		_fwds.insert(c);
	}

	for (char c : _axiom) {
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

	for (auto rule : rules) {
		if (rule.first == '\0' || rule.second[0] == '\0') {
			continue;
		}
		_rules.push_back(rule);
	}

	return true;
}

ImVec2 Lsystem::center()
{
	ImVec2 ret;
	float size = 0.f;
	for (auto prim : _prims) {
		size += prim->size();
	}

	for (auto prim : _prims) {
		ret = ret + ((prim->size() / size) * prim->center());
	}
	return ret;
}

void Lsystem::rotate(const float& angle, const ImVec2* origin)
{
	ImVec2 t;
	if (origin == nullptr) t = center();
	else t = *origin;

	for (auto prim : _prims) {
		prim->rotate(angle, &t);
	}
}

void Lsystem::scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2* origin)
{
	ImVec2 t;
	if (origin == nullptr) t = center();
	else t = *origin;

	for (auto prim : _prims) {
		prim->scale(scaleCoeffX, scaleCoeffY, &t);
	}
}

void Lsystem::translate(const ImVec2* d)
{
	for (auto prim : _prims) {
		prim->translate(d);
	}
}