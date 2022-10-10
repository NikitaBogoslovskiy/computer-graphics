#pragma once
#ifndef __LSYSTEM__
#define __LSYSTEM

#include <string>
#include <vector>
#include "pch.h"
#include <queue>
#include <stack>
#include <set>

class Lsystem
{
private:
	std::string _axiom;
	std::vector<std::pair<char, std::string>> _rules;
	std::vector<Primitive*> _fractals;
	std::vector<size_t> _fractals_iter_start;
	size_t _iters;
	std::set<char> _fwds;
	float _angle;
	bool _is_legal;
	float _line_length;
	bool _random_angle;
	ImU32 _color; 
	float _thickness;
	bool _is_tree;

public:
	Lsystem(const std::string& axiom, const std::vector<std::pair<char, std::string>>& rules, const float& angle, size_t num_iterations, const ImU32& color, const float& thickness, const std::string& additional = "", bool is_tree = false) {
		_axiom = axiom;
		_angle = angle;
		_iters = num_iterations;
		_line_length = 30.f;
		_random_angle = false;
		_color = color;
		_thickness = thickness;
		_is_tree = is_tree;
		if (_is_legal = check_Lsystem(rules, additional)) {
			calculate_fractal(_color, _thickness);
		}
	}

	inline Primitive* prim() { return _fractals[0]; }
	inline bool is_legal() { return _is_legal; }

	void calculate_fractal(const ImU32& color, const float& thickness);
	void calculate_rule(float* curr_angle, const std::string& pattern, size_t iter);
	void calculate_edge(const float& curr_angle, const size_t& iter);
	void draw(ImDrawList* draw_list, const ImVec2& offset);

private:
	bool check_Lsystem(const std::vector<std::pair<char, std::string>>& rules, const std::string& additional);
};
#endif // !__LSYSTEM__
