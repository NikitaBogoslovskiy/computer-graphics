#pragma once
#include "Sphere.h"

class LightSphere : public Sphere
{
public:
	LightSphere(const HVec<double>& inPosition, double radius, const HVec<double>& inColor);
	~LightSphere();
private:

};