#include <algorithm>
#include "geometry/primitives3d/floating_horizon.h"

/*void FloatingHorizon::_interpolateLine(ImDrawList* draw_list, const ImVec2& p0, const ImVec2& p1) {
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
}*/

void FloatingHorizon::_interpolateLine(ImDrawList* draw_list, const ImVec2& p0, const ImVec2& p1, const float& h) {
	ImVec2 p0_2d = p0;
	ImVec2 p1_2d = p1;
	float alpha = 666;
	if (p1.x - p0.x != 0)
		alpha = abs((p1.y - p0.y) / (p1.x - p0.x));
	if (alpha > 1)
	{
		std::swap(p0_2d.x, p0_2d.y);
		std::swap(p1_2d.x, p1_2d.y);
	}
	float dx = p1_2d.x - p0_2d.x;
	if (dx < 0)
	{
		std::swap(p0_2d, p1_2d);
		dx = abs(dx);
	}
	float dy = p1_2d.y - p0_2d.y;
	float sign_dy = dy > 0 ? 1 : -1;
	float di = sign_dy * 2 * dy - dx;
	float yi = 0;
	float line_length = distance(p0_2d, p1_2d);

	for (float xi = 0; xi <= dx; ++xi)
	{
		float new_x, new_y;
		if (alpha > 1)
		{
			new_x = p0_2d.y + yi;
			new_y = p0_2d.x + xi;
		}
		else
		{
			new_y = p0_2d.y + yi;
			new_x = p0_2d.x + xi;
		}
		if (new_x >= 0 && new_y >= 0 && new_x < _upperHor.size() && new_y < h)
		{
			ImVec2 p(new_x, new_y);
			if (_upperHor[(size_t)new_x] > new_y)
			{
				draw_list->AddRectFilled(p, p + ImVec2(1.f, 1.f), GetColorV4U32(upperColor));
				_upperHor[(size_t)new_x] = new_y;
			}
			if (_lowerHor[(size_t)new_x] < new_y)
			{
				draw_list->AddRectFilled(p, p + ImVec2(1.f, 1.f), GetColorV4U32(lowerColor));
				_lowerHor[(size_t)new_x] = new_y;
			}
		}
		if (di > 0)
		{
			yi += sign_dy;
			di += 2 * dy * sign_dy - 2 * dx;
		}
		else
			di += 2 * dy * sign_dy;
	}
}

void FloatingHorizon::draw(ImDrawList* draw_list, Camera& cam, const ImVec2& wh, const ImVec2& origin, const ImVec2& canvasOffset) {
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
	resize(wh.x + canvasOffset.x);
	clear();
	for (unsigned int j = 0; j <= _z_parts; j++) {
		auto z = zstart + dz * j;
		Eigen::Vector4f v1 = cam.viewProjecion() * Eigen::Vector4f{ _x0, _fnValuesBuf[j][0], z,  1.f };
		auto prevPoint = (1.f / v1(3)) * ImVec2(v1(0), v1(1)) + origin;

		for (unsigned int i = 1; i <= _x_parts; i++) {
			auto x = _x0 + dx * i;
			Eigen::Vector4f v = cam.viewProjecion() * Eigen::Vector4f{ x, _fnValuesBuf[j][i], z,  1.f };
			auto curPoint = (1.f / v(3)) * ImVec2(v(0), v(1)) + origin;
			_interpolateLine(draw_list, prevPoint, curPoint, wh.y + canvasOffset.y);
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