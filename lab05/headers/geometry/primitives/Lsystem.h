#pragma once
#ifndef __LSYSTEM__
#define __LSYSTEM

#include <string>
#include <vector>
#include "pch.h"

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

public:
	Lsystem(const std::string& axiom, const std::vector<std::pair<char, std::string>>& rules, const float& angle, size_t num_iterations);

	inline Primitive* prim() { return _fractal; }
	inline bool is_legal() { return _is_legal; }

	void calculate_fractal(const ImU32& color, const float& thickness);
	void calculate_rule(float* curr_angle, const std::string& pattern, size_t iter);
	void calculate_edge(const float& curr_angle);
	void draw(ImDrawList* draw_list, const ImVec2& offset, const ImU32& color, const float& thickness);

private:
	bool check_Lsystem(const std::string& axiom, const std::vector<std::pair<char, std::string>>& rules);
};

#endif // !__LSYSTEM__
