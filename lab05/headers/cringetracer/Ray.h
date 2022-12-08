#pragma once
#include "cringetracer/HeyraMath/HVec.h"

template <class T> class Ray
{

public:
	HVec<T> p1;
	HVec<T> p2;
	HVec<T> direction;

	Ray();
	Ray(const HVec<T>& point1, const HVec<T>& point2);
	~Ray();

	//const HVec<double>& p1() const;
	//const HVec<double>& p2() const;
	//const HVec<double>& Direction() const;
};

template<class T>
inline Ray<T>::Ray()
{
	p1 = HVec<T>{ (T)0.0, (T)0.0, (T)0.0 };
	p2 = HVec<T>{ (T)0.0, (T)0.0, (T)1.0 };
	direction = p2 - p1;
}

template<class T>
inline Ray<T>::Ray(const HVec<T>& point1, const HVec<T>& point2)
{
	p1 = point1;
	p2 = point2;
	direction = p2 - p1;
}

template<class T>
inline Ray<T>::~Ray()
{

}
