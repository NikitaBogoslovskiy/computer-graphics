#define _CRT_SECURE_NO_WARNINGS // sscanf from c language is considered bad (but it's useful). another option is istringstream from c++
#include "BLEV/Validator.h"
#include <stdio.h>
#include <stdexcept>

void BLEV::Validator::ValidateMeshGraphArgs(char* nstr, float& x0, float& x1, float& z0, float& z1, int& x_parts, int& z_parts) {
	if (sscanf(nstr, "%f %f %f %f %d %d", &x0, &x1, &z0, &z1, &x_parts, &z_parts) != 6) throw std::invalid_argument("Incorrect arguments format for interval input");
	if (x1 <= x0) throw std::invalid_argument("x1 shouldnt be lower than x0");
	if (z1 <= z0) throw std::invalid_argument("z1 shouldnt be lower than z0");
	if (x_parts <= 1) throw std::invalid_argument("x_parts shouldnt be lower than 1");
	if (z_parts <= 1) throw std::invalid_argument("z_parts shouldnt be lower than 1");
}