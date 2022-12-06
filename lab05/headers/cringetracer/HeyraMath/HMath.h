#pragma once
#include <math.h>

#define EPS 1e-21f

class HMath
{
public:
	static bool CloseEnough(const double x, const double y) {
		return fabs(x - y) < EPS;
	}
};