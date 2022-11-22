#pragma once

#include "geometry/primitives3d/mesh_graph.h"
#include "pch.h"
#include <array>
#include <iostream>

using Horizon = std::vector<float>;

class FloatingHorizon
{
	Horizon _lowerHor;
	Horizon _upperHor;

	float _x0;
	float _x1;
	float _z0;
	float _z1;
	size_t _x_parts;
	size_t _z_parts;
	Func3d _func;

	void _interpolateLine(const Eigen::Vector4f& p0, const Eigen::Vector4f& p1, const ImVec2& offset);
public:
	ImVec4 lowerColor;
	ImVec4 upperColor;
	FloatingHorizon(const float& x0, const float& x1,
		const float& z0, const float& z1,
		const size_t& x_parts,
		const size_t& z_parts,
		Func3d func,
		const ImVec4& upperColor = ImVec4(204.f, 201.f, 230.f, 255),
		const ImVec4& lowerColor = ImVec4(62.f, 72.f, 157.f, 255)) {
		this->lowerColor = lowerColor;
		this->upperColor = upperColor;
		this->_x0 = x0;
		this->_x1 = x1;
		this->_z0 = z0;
		this->_z1 = z1;
		this->_x_parts = x_parts;
		this->_z_parts = z_parts;
		this->_func = func;
	}
	inline void resize(const size_t& width);
	inline void clear();
	void draw(ImDrawList* draw_list, const ImVec2& offset, Camera& cam);
};
