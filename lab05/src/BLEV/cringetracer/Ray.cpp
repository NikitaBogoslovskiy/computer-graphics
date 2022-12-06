#include "../headers/cringetracer/Ray.h"

Ray::Ray(const HVec<double>& point1, const HVec<double>& point2)
{
	_p1 = point1;
	_p2 = point2;
	_direction = _p2 - _p1;
}

const HVec<double>& Ray::p1() const
{
	return _p1;
}

const HVec<double>& Ray::p2() const
{
	return _p2;
}

const HVec<double>& Ray::Direction() const
{
	return _direction;
}

Ray::~Ray()
{
}
