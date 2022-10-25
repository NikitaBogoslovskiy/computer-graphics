#ifndef __LSYSTEM__
#define __LSYSTEM__

#include "pch.h"
#include "primitive.h"
#include "geometry/methods/funcs.h"
#include <string>
#include <vector>
#include <set>

class Lsystem
{
private:
	std::string _axiom;
	std::vector<std::pair<char, std::string>> _rules;
	std::vector<Primitive*> _prims;
	std::vector<size_t> _prims_iter_start;
	size_t _iters;
	std::set<char> _fwds;
	float _angle;
	bool _is_legal;
	float _line_length;
	bool _random_angle;
	ImVec4 _src_color; 
	ImVec4 _dest_color; 
	float _thickness;
	bool _is_tree;
	bool _show;

	bool _is_alive;

public:
	Lsystem(const std::string& axiom, 
		const std::vector<std::pair<char, std::string>>& rules, 
		const float& angle, size_t num_iterations, 
		const ImVec4& src_color, 
		const float& thickness, 
		const ImVec4& dest_color = ImVec4(),
		const std::string& additional = "", 
		bool is_tree = false) {
		_axiom = axiom;
		_angle = angle;
		_iters = num_iterations;
		_line_length = 30.f;
		_random_angle = false;
		_src_color = src_color;
		_dest_color = dest_color;
		_thickness = thickness;
		_is_tree = is_tree;
		_show = true;
		_is_alive = false;
		if (_is_legal = check_Lsystem(rules, additional)) {
			calculate_fractal(GetColorV4U32(_src_color), _thickness);
		}
	}

	inline std::vector<Primitive*> prims() { return _prims; }
	inline bool is_legal() { return _is_legal; }

	void calculate_fractal(const ImU32& color, const float& thickness);
	void calculate_rule(float* curr_angle, const std::string& pattern, size_t iter);
	void calculate_edge(const float& curr_angle, const size_t& iter);
	void draw(ImDrawList* draw_list, const ImVec2& offset);
	inline bool& show() { return _show; }
	inline const bool& is_tree() { return _is_tree; }
	inline bool& is_alive() { return _is_alive; }
	inline ImVec4& src_col() { return _src_color; }
	inline ImVec4& dest_col() { return _dest_color; }
	inline float& thickness() { return _thickness; }

	void rotate(const float& angle, const ImVec2* origin);
	void scale(const float& scaleCoeffX, const float& scaleCoeffY, const ImVec2* origin);
	void translate(const ImVec2* d);
	
private:
	bool check_Lsystem(const std::vector<std::pair<char, std::string>>& rules, const std::string& additional);
	ImVec2 center();
};
#endif // !__LSYSTEM__
