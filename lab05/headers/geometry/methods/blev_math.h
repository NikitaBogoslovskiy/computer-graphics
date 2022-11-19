#pragma once

#include <algorithm> // cos, sin

typedef const float (*Func3d)(const float&, const float&);

// for miscellaneous math functions without specific purpose
class BLEVmath
{
public:
	inline static const float ripples(const float& x, const float& z) {
		float r = x * x + z * z + 1;
		return 5 * (cos(r) / r + 0.1);
	}
	inline static const float ripples2(const float& x, const float& z) {
		float r = x * x + z * z;
		return cos(r) / (r + 1);
	}
	inline static const float sinxMultCosz(const float& x, const float& z) {
		return sin(x) * cos(z);
	}
	inline static const float sinxPlusCosz(const float& x, const float& z) {
		return sin(x) + cos(z);
	}
	inline static const float squaresSum(const float& x, const float& z) {
		return x * x + z * z;
	}
};