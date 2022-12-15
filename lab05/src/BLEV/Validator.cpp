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

void BLEV::Validator::ValidateSphereArgs(char* nstr, double& x0, double& y0, double& z0, double& radius) {
	if (sscanf(nstr, "%lf %lf %lf %lf", &x0, &y0, &z0, &radius) != 4) throw std::invalid_argument("Incorrect arguments format for sphere");
	if (radius <= 0) throw std::invalid_argument("radius shouldnt be <= 0");
}

//void BLEV::Validator::ValidateEllipsoidArgs(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ) {
//	if (sscanf(nstr, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &rX, &rY, &rZ) != 6) throw std::invalid_argument("Incorrect arguments format for ellipsoid");
//	if (rX <= 0) throw std::invalid_argument("rX shouldnt be <= 0");
//	if (rY <= 0) throw std::invalid_argument("rY shouldnt be <= 0");
//	if (rZ <= 0) throw std::invalid_argument("rZ shouldnt be <= 0");
//}

void BLEV::Validator::ValidatePlaneArgs(char* nstr, double& x0, double& y0, double& z0, double& u, double& v) {
	if (sscanf(nstr, "%lf %lf %lf %lf %lf", &x0, &y0, &z0, &u, &v) != 5) throw std::invalid_argument("Incorrect arguments format for plane");
	if (u <= 0) throw std::invalid_argument("u shouldnt be <= 0");
	if (v <= 0) throw std::invalid_argument("v shouldnt be <= 0");
}

void BLEV::Validator::ValidateOS(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ)
{
	if (sscanf(nstr, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &rX, &rY, &rZ) != 6) throw std::invalid_argument("Incorrect arguments format for box");
	if (rX <= 0) throw std::invalid_argument("rX shouldnt be <= 0");
	if (rY <= 0) throw std::invalid_argument("rY shouldnt be <= 0");
	if (rZ <= 0) throw std::invalid_argument("rZ shouldnt be <= 0");
}

//void BLEV::Validator::ValidateBoxArgs(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ) {
//	if (sscanf(nstr, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &rX, &rY, &rZ) != 6) throw std::invalid_argument("Incorrect arguments format for box");
//	if (rX <= 0) throw std::invalid_argument("rX shouldnt be <= 0");
//	if (rY <= 0) throw std::invalid_argument("rY shouldnt be <= 0");
//	if (rZ <= 0) throw std::invalid_argument("rZ shouldnt be <= 0");
//}
//
//void BLEV::Validator::ValidateCylinderArgs(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ) {
//	if (sscanf(nstr, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &rX, &rY, &rZ) != 6) throw std::invalid_argument("Incorrect arguments format for cylinder");
//	if (rX <= 0) throw std::invalid_argument("rX shouldnt be <= 0");
//	if (rY <= 0) throw std::invalid_argument("rY shouldnt be <= 0");
//	if (rZ <= 0) throw std::invalid_argument("rZ shouldnt be <= 0");
//}


void BLEV::Validator::ValidateLightArgs(char* nstr, double& inPitch, double& inYaw, double& inR, double& inIntensity) {
	if (sscanf(nstr, "%lf %lf %lf %lf", &inPitch, &inYaw, &inR, &inIntensity) != 4) throw std::invalid_argument("Incorrect argument amount for light");
	if (inIntensity <= 0) throw std::invalid_argument("intensity shouldnt be <= 0");
}