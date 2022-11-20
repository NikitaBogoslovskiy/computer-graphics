#pragma once

#include <iostream>
#include "pch.h"

using byte = unsigned char;

class Clipper
{
	static constexpr const byte RegionCodes[] = { 1, 2, 4, 8, 16, 32 }; // left, right, bottom, top, near, far
	static constexpr const byte RegionIndex[] = { 0, 0, 1, 1, 2, 2 };
	//static constexpr const char RegionSign[] = { -1, 1, -1, 1, -1, 1 };
	//static constexpr const char Coef[] = { 1, 1, 1, 1, 0, 1 };
	static constexpr const char Planes[] = { -1, 1, -1, 1, 0, 1 }; // left, right, bottom, top, near, far

	static Eigen::Vector4f interpolate(const Eigen::Vector4f& p0, const Eigen::Vector4f& p1, const float& t) {
		return p0 + t * (p1 - p0);
	}

	static bool isPointInNDC(const Eigen::Vector4f& p) {
		return (-p(3) <= p(0)) && (p(0) <= p(3))
			&& (-p(3) <= p(1)) && (p(1) <= p(3))
			&& (0 <= p(2)) && (p(2) <= p(3));
	}

	static byte getRegionCode(const Eigen::Vector4f& p) {
		byte outcode = 0x00;
		if (p(0) > p(3)) {
			outcode = outcode | RegionCodes[1];// RIGHT
		}
		else {
			if (p(0) < -p(3))
				outcode = outcode | RegionCodes[0];// LEFT
		}
		if (p(1) > p(3)) {
			outcode = outcode | RegionCodes[3];// TOP
		}
		else {
			if (p(1) < -p(3))
				outcode = outcode | RegionCodes[2];// BOTTOM
		}
		if (p(2) > p(3)) {
			outcode = outcode | RegionCodes[5];// FAR
		}
		else {
			if (p(2) < 0)
				outcode = outcode | RegionCodes[4];// NEAR
		}
		return outcode;
	}
public:
	static bool clipLineSegment(Eigen::Vector4f& p0, Eigen::Vector4f& p1) {
		if ((p0(3) < 0) && (p1(3) < 0)) return false;

		auto points = std::vector<Eigen::Vector4f*>{ &p0, &p1 };
		auto code = std::vector<byte>{ getRegionCode(p0), getRegionCode(p1) };

		if (!(code[0] | code[1])) return true;
		if (code[0] & code[1]) return false;

		Eigen::Vector4f p;
		bool accept = false;
		size_t pointToAdjust = 0;
		//if (code[0]) {}
		//else { pointToAdjust = 1; }

		while (true) {
			//if (!(code[0] | code[1])) {
			if ((code[0]==0) && (code[1]==0)) {
				accept = true;
				break;
			}
			if (code[0] & code[1]) {
				break;
			}
			if (code[0]) { pointToAdjust = 0; }
			else { pointToAdjust = 1; }
			//static constexpr const byte RegionCodes[] = { 1, 2, 4, 8, 16, 32 }; // left, right, bottom, top, near, far
			//static constexpr const char RegionSign[] = { -1, 1, -1, 1, -1, 1 };
			//static constexpr const byte RegionIndex[] = { 0, 0, 1, 1, 2, 2 };
			for (size_t r = 0; r < 6; r++) { // order of edges based on priority
				if (!(code[pointToAdjust] & RegionCodes[r])) continue;
				std::cout << "before: " << (*points[pointToAdjust])(0) << " " << (*points[pointToAdjust])(1) << " " << (*points[pointToAdjust])(2) << " " << (*points[pointToAdjust])(3) << " " << std::endl;
				auto ri = RegionIndex[r];
				float plane = (*points[pointToAdjust])(3) * Planes[r];
				float t = (plane - (*points[0])(ri)) / ((*points[1])(ri) - (*points[0])(ri));
				//float d1 = Coef[ri] * RegionSign[r] * (*points[0])(3) + RegionSign[r] * (*points[0])(ri);
				//float d2 = Coef[ri] * RegionSign[r] * (*points[1])(3) + RegionSign[r] * (*points[1])(ri);
				//*points[pointToAdjust] = interpolate((*points[0]), (*points[1]), d1 / (d2 - d1));
				(*points[pointToAdjust]) = interpolate((*points[0]), (*points[1]), t);
				(*points[pointToAdjust])(3) = abs((*points[pointToAdjust])(ri));
				code[pointToAdjust] = getRegionCode(*points[pointToAdjust]);
				std::cout << "after adjusting: " << (*points[pointToAdjust])(0) << " " << (*points[pointToAdjust])(1) << " " << (*points[pointToAdjust])(2) << " " << (*points[pointToAdjust])(3) << " " << std::endl << std::endl;
				break;
			}
		}
		return accept;
	}

	/*static bool clipLineSegment(Eigen::Vector4f& p0, Eigen::Vector4f& p1) {
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
	}*/
};
