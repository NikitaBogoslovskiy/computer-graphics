#pragma once

#include <iostream>
#include "pch.h"

using byte = unsigned char;

class Clipper
{
	static constexpr const byte RegionCodes[] = { 1, 2, 4, 8, 16, 32 };
	static constexpr const byte RegionIndex[] = { 0, 0, 1, 1, 2, 2 };
	static constexpr const char RegionSign[] = { 1, -1, 1, -1, -1, -1 };

	// calculates region code for point p
	static byte getRegionCode(const Eigen::Vector4f& p) {
		byte outcode = 0x00;

		if (p(0) < -p(3)) {
			outcode = outcode | RegionCodes[1];// RIGHT
		}
		else {
			if (p(0) > p(3))
				outcode = outcode | RegionCodes[0];// LEFT
		}

		if (p(1) < -p(3)) {
			outcode = outcode | RegionCodes[3];// TOP
		}
		else {
			if (p(1) > p(3))
				outcode = outcode | RegionCodes[2];// BOTTOM
		}

		//if (p(2) > p(3)) {
		//	outcode = outcode | RegionCodes[5];// FAR
		//}
		//else {
		//	if (p(2) < -p(3))
		//		outcode = outcode | RegionCodes[4];// NEAR
		//}
		if (p(2) < -p(3)) {
			outcode = outcode | RegionCodes[5];// FAR
		}
		else {
			if (p(2) > p(3))
				outcode = outcode | RegionCodes[4];// NEAR
		}
		return outcode;
	}

	static Eigen::Vector4f interpolate(const Eigen::Vector4f& p0, const Eigen::Vector4f& p1, const float& t) {
		return p0 + t * (p1 - p0);
	}
public:
	enum Cumclusion {
		Clipped,
		Removed,
		FullyDrawn
	};
	static Cumclusion cumclusions;

	static Cumclusion clipLineSegment(Eigen::Vector4f& p0, Eigen::Vector4f& p1) {
		if ((p0(3) < 0) && (p1(3) < 0)) return Removed;

		byte outCode0 = getRegionCode(p0);
		byte outCode1 = getRegionCode(p1);

		if (!(outCode0 | outCode1)) return FullyDrawn;
		if (outCode0 & outCode1) return Removed;

		Eigen::Vector4f p{ 0.0f, 0.0f, 0.0f, 0.0f };
		while (true) {
			//std::cout << "sasas" << std::endl;
			if (!(outCode0 | outCode1)) return Clipped;
			byte outCode;
			if (outCode0) { // start with first non zero outcode(order 0 to 1)
				outCode = outCode0;
			}
			else outCode = outCode1;
			for (size_t r = 0; r < 6; r++) { // order of edges based on priority 
				if (!(outCode & RegionCodes[r])) continue;
				float d1 = p0(3) + RegionSign[r] * p0(RegionIndex[r]);
				float d2 = p1(3) + RegionSign[r] * p1(RegionIndex[r]);
				p = interpolate(p0, p1, d1 / (d2 - d1));
				p(3) = abs(p(RegionIndex[r]));
				break;
			}
			auto pCode = getRegionCode(p);
			if (outCode == outCode0) {
				outCode0 = pCode;
				p0 = p;
			}
			else {
				outCode0 = pCode;
				p1 = p;
			}

		}
	}
};

/*
static std::string stringCumclusion(Cumclusion cum) {
		switch (cum)
		{
		case Clipped:
			return "Clipped";
		case Removed:
			return "Removed";
		case FullyDrawn:
			return " ======================> FullyDrawn";
		default:
			return "Sorry...";
		}
	}*/