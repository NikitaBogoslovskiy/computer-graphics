#pragma once

#include <iostream>
#include "pch.h"

using byte = unsigned char;

class Clipper
{
	static Eigen::Vector4f interpolate(const Eigen::Vector4f& p0, const Eigen::Vector4f& p1, const float& t) {
		return p0 + t * (p1 - p0);
	}

	static bool isPointInNDC(const Eigen::Vector4f& p) {
		return (-p(3) <= p(0)) && (p(0) <= p(3))
			&& (-p(3) <= p(1)) && (p(1) <= p(3))
			&& (0 <= p(2)) && (p(2) <= p(3));
	}
public:

	static bool clipLineSegment(Eigen::Vector4f& p0, Eigen::Vector4f& p1) {
		auto p0_InNdc = isPointInNDC(p0);
		auto p1_InNdc = isPointInNDC(p1);
		if (!p0_InNdc && !p1_InNdc) return false;
		if (p0_InNdc && p1_InNdc) return true;

		if (!p1_InNdc) {
			std::swap(p0, p1);
		}
		auto z0 = p0(2);
		auto z1 = p1(2);
		const float t = -z0 / (z1 - z0);
		p0 = interpolate(p0, p1, t);
		return true;
	}
};