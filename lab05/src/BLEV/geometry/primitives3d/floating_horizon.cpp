#include <algorithm>
#include "geometry/primitives3d/floating_horizon.h"

void FloatingHorizon::_interpolateLine(ImDrawList* draw_list, const ImVec2& p0, const ImVec2& p1) {
	//draw_list->AddRectFilled(ImVec2(x, y) + canvasOffset, ImVec2(x + 1, y + 1) + canvasOffset, GetColorV4U32(colorBuffer[y][x]));
	float len = std::max(abs(p1.x - p0.x), abs(p1.y - p0.y));
	float maxacc = 2 * len;
	float accx = len;
	float accy = len;

	float x0 = p0.x;
	float x1 = p1.x;
	float y0 = p0.y;
	float y1 = p1.y;

	for (int j = 0; j < 2; j++)
	{
		float dx = 2 * (p1.x - p0.x);
		float dy = 2 * (p1.y - p0.y);
		float x = p0.x;
		float y = p0.y;
		len += j;

		for (int i = 0; i < len; i++)
		{
			if (0 <= x && x < _upperHor.size()) {
				ImVec2 p(x, y);
				if (y0 <= y1 && _upperHor[(size_t)x] > y)
				{
					draw_list->AddRectFilled(p, p + ImVec2(1.f, 1.f), GetColorV4U32(upperColor));
					_upperHor[(size_t)x] = y;
				}
				if (y0 >= y1 && _lowerHor[(size_t)x] < y)
				{
					draw_list->AddRectFilled(p, p + ImVec2(1.f, 1.f), GetColorV4U32(lowerColor));
					_lowerHor[(size_t)x] = y;
				}
			}

			accx += dx; accy += dy;
			if (accx >= maxacc)
			{
				accx -= maxacc; x++;
			}
			else if (accx < 0)
			{
				accx += maxacc; x--;
			}
			if (accy >= maxacc)
			{
				accy -= maxacc; y++;
			}
			else if (accy < 0)
			{
				accy += maxacc; y--;
			}
		}

		float temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
}

void FloatingHorizon::draw(ImDrawList* draw_list, Camera& cam, const float& width, const ImVec2& origin, const ImVec2& canvasOffset) {
	float zstart = abs(cam.eye().z - _z0) < abs(cam.eye().z - _z1) ? _z0 : _z1;
	float dx = abs(_x0 - _x1) / _x_parts,
		  dz = (_z0 + _z1 - 2.f * zstart) / _z_parts;
	for (unsigned int j = 0; j <= _z_parts; j++) {
		auto z = zstart + dz * j;
		for (unsigned int i = 0; i <= _x_parts; i++) {
			auto x = _x0 + dx * i;
			_fnValuesBuf[j][i] = _func(x, z);
		}
	}
	resize(width + canvasOffset.x);
	clear();
	for (unsigned int j = 0; j <= _z_parts; j++) {
		auto z = zstart + dz * j;
		Eigen::Vector4f v1 = cam.viewProjecion() * Eigen::Vector4f{ _x0, _fnValuesBuf[j][0], z,  1.f };
		auto prevPoint = (1.f / v1(3)) * ImVec2(v1(0), v1(1)) + origin;

		for (unsigned int i = 1; i <= _x_parts; i++) {
			auto x = _x0 + dx * i;
			Eigen::Vector4f v = cam.viewProjecion() * Eigen::Vector4f{ x, _fnValuesBuf[j][i], z,  1.f };
			auto curPoint = (1.f / v(3)) * ImVec2(v(0), v(1)) + origin;
			_interpolateLine(draw_list, prevPoint, curPoint);
			prevPoint = curPoint;
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