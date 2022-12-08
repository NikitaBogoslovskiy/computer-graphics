#pragma once
#include "HVec.h"
#include "../Ray.h"

#define FORWARD true
#define BACKWARD false

template <class T> class HAffine
{
public:
	HAffine();
	HAffine(const Eigen::Matrix<T, 4, 4>& toGlobal, const Eigen::Matrix<T, 4, 4>& toLocal);
	HAffine(const HVec<T>& translation, const HVec<T>& rotation, const HVec<T>& scale);

	~HAffine();

	void SetTransform(const HVec<T>& translation, const HVec<T>& rotation, const HVec<T>& scale);

	const Eigen::Matrix<T, 4, 4>& Forward();
	const Eigen::Matrix<T, 4, 4>& Backward();

	static Eigen::Matrix<T, 4, 4> Translation(const HVec<T>& inVec);
	static Eigen::Matrix<T, 4, 4> RotationX(const T& angle);
	static Eigen::Matrix<T, 4, 4> RotationY(const T& angle);
	static Eigen::Matrix<T, 4, 4> RotationZ(const T& angle);
	static Eigen::Matrix<T, 4, 4> Scale(const HVec<T>& inVec);

	HVec<T> Transform(const HVec<T>& inVec, bool toGlobal);
	Ray<T> Transform(const Ray<T>& inRay, bool toGlobal);

	friend HAffine<T> operator* (const HAffine<T>& lhs, const HAffine<T>& rhs);

	HAffine operator= (const HAffine& rhs);

protected:

	Eigen::Matrix<T, 4, 4> _toGlobal, _toLocal;
};

template<class T> HAffine<T> operator* (const HAffine<T>& lhs, const HAffine<T>& rhs) {
	Eigen::Matrix<T, 4, 4> newGlobal = lhs._toGlobal * rhs._toGlobal;
	Eigen::Matrix<T, 4, 4> newLocal = newGlobal.inverse();
	return HAffine<T>(newGlobal, newLocal);
}

template<class T>
inline HAffine<T> HAffine<T>::operator=(const HAffine& rhs)
{
	if (this != &rhs)
	{
		_toGlobal = rhs._toGlobal;
		_toLocal = rhs._toLocal;
	}

	return *this;
}

template <class T> HAffine<T>::HAffine()
{
	_toGlobal.setIdentity();
	_toLocal.setIdentity();
}

template<class T>
inline HAffine<T>::HAffine(const Eigen::Matrix<T, 4, 4>& toGlobal, const Eigen::Matrix<T, 4, 4>& toLocal)
{
	_toGlobal = toGlobal;
	_toLocal = toLocal;
}

template<class T>
inline HAffine<T>::HAffine(const HVec<T>& translation, const HVec<T>& rotation, const HVec<T>& scale)
{
	this->SetTransform(translation, rotation, scale);
}

template <class T> HAffine<T>::~HAffine()
{

}

// ============================================================

template <class T> void HAffine<T>::SetTransform(const HVec<T>& translation, const HVec<T>& rotation, const HVec<T>& scale)
{
	_toGlobal = HAffine<T>::Translation(translation)
		* HAffine<T>::RotationX(rotation.At(0))
		* HAffine<T>::RotationY(rotation.At(1))
		* HAffine<T>::RotationZ(rotation.At(2))
		* HAffine<T>::Scale(scale);
	_toLocal = _toGlobal.inverse();
}

template<class T>
inline HVec<T> HAffine<T>::Transform(const HVec<T>& inVec, bool toGlobal)
{
	Eigen::Matrix<T, 4, 1> hVec{ inVec.At(0), inVec.At(1), inVec.At(2), (T)1 };
	Eigen::Matrix<T, 4, 1> temp;
	if (toGlobal) {
		temp = _toGlobal * hVec;
	}
	else {
		temp = _toLocal * hVec;
	}
	return HVec<T> { temp(0, 0), temp(1, 0), temp(2, 0) };
}

template<class T>
inline Ray<T> HAffine<T>::Transform(const Ray<T>& inRay, bool toGlobal)
{
	Ray<T> outRay;
	outRay.p1 = HAffine<T>::Transform(inRay.p1, toGlobal);
	outRay.p2 = HAffine<T>::Transform(inRay.p2, toGlobal);
	outRay.direction = outRay.p2 - outRay.p1;
	return outRay;
}

// ============================================================

template<class T>
inline Eigen::Matrix<T, 4, 4> HAffine<T>::Translation(const HVec<T>& inVec)
{
	return Eigen::Matrix<T, 4, 4>{
		{ (T)1, (T)0, (T)0, inVec.At(0) },
		{ (T)0, (T)1, (T)0, inVec.At(1) },
		{ (T)0, (T)0, (T)1, inVec.At(2) },
		{ (T)0, (T)0, (T)0, (T)1 }
	};
}

template<class T>
inline Eigen::Matrix<T, 4, 4> HAffine<T>::RotationX(const T& angle)
{
	return Eigen::Matrix<T, 4, 4>{
		{ (T)1, (T)0, (T)0, (T)0 },
		{ (T)0,  (T)cos(angle), (T)sin(angle), (T)0 },
		{ (T)0, -(T)sin(angle), (T)cos(angle), (T)0 },
		{ (T)0,  (T)0,			(T)0,		   (T)1 }
	};
}

template<class T>
inline Eigen::Matrix<T, 4, 4> HAffine<T>::RotationY(const T& angle)
{
	return Eigen::Matrix<T, 4, 4>{
		{ (T)cos(angle), (T)0, -(T)sin(angle), (T)0 },
		{ (T)0,			 (T)1,  (T)0,		   (T)0 },
		{ (T)sin(angle), (T)0,  (T)cos(angle), (T)0 },
		{ (T)0,			 (T)0,  (T)0,		   (T)1 }
	};
}

template<class T>
inline Eigen::Matrix<T, 4, 4> HAffine<T>::RotationZ(const T& angle)
{
	return Eigen::Matrix<T, 4, 4>{
		{  (T)cos(angle), (T)sin(angle), (T)0, (T)0 },
		{ -(T)sin(angle), (T)cos(angle), (T)0, (T)0 },
		{ (T)0,		  (T)0,			 (T)1, (T)0 },
		{ (T)0,		  (T)0,			 (T)0, (T)1 }
	};
}

template<class T>
inline Eigen::Matrix<T, 4, 4> HAffine<T>::Scale(const HVec<T>& inVec)
{
	return Eigen::Matrix<T, 4, 4>{
		{ inVec.At(0), (T)0, (T)0, (T)0 },
		{ (T)0,		   inVec.At(1), (T)0,		 (T)0 },
		{ (T)0,		   (T)0,		inVec.At(2), (T)0 },
		{ (T)0,		   (T)0,		(T)0,		 (T)1 }
	};
}
// ============================================================

template<class T>
inline const Eigen::Matrix<T, 4, 4>& HAffine<T>::Forward()
{
	return _toGlobal;
}

template<class T>
inline const Eigen::Matrix<T, 4, 4>& HAffine<T>::Backward()
{
	return _toLocal;
}
