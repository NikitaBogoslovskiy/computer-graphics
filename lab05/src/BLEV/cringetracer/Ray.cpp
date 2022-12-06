#include "../headers/cringetracer/Ray.h"

Ray::Ray()
{
	p1 = HVec<double>{ 0.0, 0.0, 0.0 };
	p2 = HVec<double>{ 0.0, 0.0, 1.0 };
	direction = p2 - p1;
}

Ray::Ray(const HVec<double>& point1, const HVec<double>& point2)
{
	p1 = point1;
	p2 = point2;
	direction = p2 - p1;
}

//const HVec<double>& Ray::p1() const
//{
//	return _p1;
//}
//
//const HVec<double>& Ray::p2() const
//{
//	return _p2;
//}
//
//const HVec<double>& Ray::Direction() const
//{
//	return _direction;
//}

Ray::~Ray()
{
}
