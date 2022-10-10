#pragma once
#ifndef __LSYSTEM__
#define __LSYSTEM

#include <string>
#include <vector>
#include "pch.h"
#include <queue>

class Lsystem
{
private:
	std::string _axiom;
	std::vector<std::pair<char, std::string>> _rules;
	Primitive* _fractal;
	size_t _iters;
	char* _fwds;
	float _angle;
	bool _is_legal;
	float _line_length;
	bool _random_angle;

public:
	Lsystem(const std::string& axiom, const std::vector<std::pair<char, std::string>>& rules, const float& angle, size_t num_iterations, const ImU32& color, const float& thickness) {
		this->_angle = angle;
		this->_iters = num_iterations;
		_line_length = 30.f;
		_random_angle = false;
		if (_is_legal = check_Lsystem(axiom, rules)) {
			calculate_fractal(color, thickness);
		}
	}

	inline Primitive* prim() { return _fractal; }
	inline bool is_legal() { return _is_legal; }

	void calculate_fractal(const ImU32& color, const float& thickness);
	void calculate_rule(float* curr_angle, const std::string& pattern, size_t iter);
	void calculate_edge(const float& curr_angle);
	void draw(ImDrawList* draw_list, const ImVec2& offset);

private:
	bool check_Lsystem(const std::string& axiom, const std::vector<std::pair<char, std::string>>& rules);
};
#endif // !__LSYSTEM__
