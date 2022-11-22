#include <algorithm>
#include "geometry/primitives3d/floating_horizon.h"

void FloatingHorizon::_interpolateLine(const Eigen::Vector4f& p0, const Eigen::Vector4f& p1, const ImVec2& offset) {
	//draw_list->AddRectFilled(ImVec2(x, y) + canvasOffset, ImVec2(x + 1, y + 1) + canvasOffset, GetColorV4U32(colorBuffer[y][x]));
}

void FloatingHorizon::draw(ImDrawList* draw_list, const ImVec2& offset, Camera& cam) {
	float dx = abs(_x0 - _x1) / _x_parts,
		dz = abs(_z0 - _z1) / _z_parts;

	for (unsigned int j = 0; j <= _z_parts; j++) {
		auto z = _z0 + dz * j;
		for (unsigned int i = 0; i <= _x_parts; i++) {
			auto x = _x0 + dx * i;
			auto y = _func(x, z);

			Eigen::Vector4f v0_2d = cam.viewProjecion() * Eigen::Vector4f{ x, y, z,  1.f };
			auto temp = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3)) + offset;
			draw_list->AddRectFilled(temp, temp + ImVec2(1.f, 1.f), GetColorV4U32(upperColor));
		}
	}
}

inline void FloatingHorizon::resize(const size_t& width) {
	_upperHor.resize(width);
	_lowerHor.resize(width);
}

inline void FloatingHorizon::clear() {
	std::fill(_upperHor.begin(), _upperHor.end(), FLT_MAX);
	std::fill(_lowerHor.begin(), _lowerHor.end(), FLT_MIN);
}