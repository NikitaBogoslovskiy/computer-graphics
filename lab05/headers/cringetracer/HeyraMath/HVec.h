#pragma once

#include "../../geometry/primitives3d/structers.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include "Eigen/Eigen"
// i was curious about overloads. so here we have
template <class T> class HVec
{
	std::vector<T> _data; // lets try to use std::array instead of vector someday. dims should be templated as well. will be faster than vector
	size_t _dims;

public:
	HVec();
	~HVec();

	HVec(size_t numDims);
	HVec(const std::vector<T>& inputData);
	HVec(const std::initializer_list<T>& inputData);
	HVec(const ImVec2& imvec);
	HVec(const ImVec3& imvec);
	HVec(const ImVec4& imvec);

	size_t GetDimensions() const;

	const T& At(const size_t index) const;
	void SetAt(const size_t index, T value);

	T len() const;
	HVec<T> Normalized() const;
	void Normalize();

	HVec<T> operator+ (const HVec<T>& rhs) const;
	HVec<T> operator- (const HVec<T>& rhs) const;
	HVec<T> operator* (const HVec<T>& rhs) const;
	HVec<T> operator* (const T& rhs) const;

	HVec<T>& operator*=(const T& rhs);
	HVec<T>& operator+=(const T& rhs);
	HVec<T>& operator-=(const T& rhs);

	HVec<T>& operator*=(const HVec<T>& rhs);
	HVec<T>& operator+=(const HVec<T>& rhs);
	HVec<T>& operator-=(const HVec<T>& rhs);

	//Eigen::Matrix<T, Eigen::Dynamic, 1> ToEigenVec();
	/*
	todo takoe
	 inline Polygon& operator=(const Polygon& _polygon) {
		indices = _polygon.indices;
		uv_ind = _polygon.uv_ind;
		normal = _polygon.normal;
		return *this;
	}
	inline Polygon& operator=(Polygon&& _polygon) noexcept {
		indices = std::move(_polygon.indices);
		uv_ind = std::move(_polygon.uv_ind);
		normal = std::move(_polygon.normal);
		return *this;
	}
	*/
	template <class U> friend HVec<U> operator* (const U& lhs, const HVec<U>& rhs);

	static T dot(const HVec<T>& a, const HVec<T>& b);
	static HVec<T> cross(const HVec<T>& a, const HVec<T>& b);
};

// =========================================== constructors

template <class T> HVec<T>::HVec()
{
	_dims = 0;
	_data = std::vector<T>();
}

template <class T> HVec<T>::~HVec()
{}

template <class T> HVec<T>::HVec(size_t numDims)
{
	_dims = numDims;
	_data = std::vector<T>(numDims, static_cast<T>(0.0));
}

template <class T> HVec<T>::HVec(const std::vector<T>& inputData) {
	_dims = inputData.size();
	_data = inputData;
}

template <class T> HVec<T>::HVec(const std::initializer_list<T>& inputData) {
	_dims = inputData.size();
	_data = inputData;
}

template <class T> HVec<T>::HVec(const ImVec2& imvec) {
	_dims = 2;
	_data = std::vector<T>{ (T)imvec.x, (T)imvec.y };
}

template <class T> HVec<T>::HVec(const ImVec3& imvec) {
	_dims = 3;
	_data = std::vector<T>{ (T)imvec.x, (T)imvec.y, (T)imvec.z };
}

template <class T> HVec<T>::HVec(const ImVec4& imvec) {
	_dims = 4;
	_data = std::vector<T>{ (T)imvec.x, (T)imvec.y, (T)imvec.z, (T)imvec.w };
}

// =========================================== properties

template <class T>
size_t HVec<T>::GetDimensions() const {
	return _dims;
}

template <class T>
const T& HVec<T>::At(size_t index) const
{
	return _data[index];
}

template <class T>
void HVec<T>::SetAt(const size_t index, T value)
{
	_data[index] = value;
}

template <class T>
T HVec<T>::len() const
{
	T cumulativeSum = static_cast<T>(0.0);
	for (size_t i = 0; i < _dims; ++i)
		cumulativeSum += (_data.at(i) * _data.at(i));

	return sqrt(cumulativeSum);
}

// ===========================================

template <class T>
HVec<T> HVec<T>::Normalized() const
{
	T vecNorm = this->len();
	return HVec<T>(_data) * (static_cast<T>(1.0) / vecNorm);
}

template <class T>
void HVec<T>::Normalize()
{
	T vecNorm = this->len();
	for (size_t i = 0; i < _dims; ++i)
	{
		_data.at(i) = _data.at(i) * (static_cast<T>(1.0) / vecNorm);
	}
}

template <class T>
HVec<T> HVec<T>::operator+ (const HVec<T>& rhs) const
{
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions do not match.");

	HVec<T> result(_dims);
	for (size_t i = 0; i < _dims; ++i)
		result.SetAt(i, _data.at(i) + rhs._data.at(i));
	return result;
}

template <class T>
HVec<T> HVec<T>::operator- (const HVec<T>& rhs) const
{
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions do not match.");

	HVec<T> result(_dims);
	for (size_t i = 0; i < _dims; ++i)
		result.SetAt(i, _data.at(i) - rhs._data.at(i));
	return result;
}

template<class T>
inline HVec<T> HVec<T>::operator*(const HVec<T>& rhs) const
{
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions do not match.");

	HVec<T> result(_dims);
	for (size_t i = 0; i < _dims; ++i)
		result.SetAt(i, _data.at(i) * rhs._data.at(i));
	return result;
}

template <class T>
HVec<T> HVec<T>::operator* (const T& rhs) const
{
	HVec<T> result(_dims);
	for (size_t i = 0; i < _dims; ++i)
		result.SetAt(i, _data.at(i) * rhs);
	return result;
}

template <class T>
HVec<T> operator* (const T& lhs, const HVec<T>& rhs)
{
	HVec<T> result(rhs._dims);
	for (size_t i = 0; i < rhs._dims; ++i)
		result.SetAt(i, lhs * rhs._data.at(i));
	return result;
}

// ========================================== the best part 

template <class T>
HVec<T>& HVec<T>::operator*=(const T& rhs) {
	for (size_t i = 0; i < _dims; ++i)
		_data[i] = _data[i] * rhs;
	return *this;
}

template <class T>
HVec<T>& HVec<T>::operator+=(const T& rhs) {
	for (size_t i = 0; i < _dims; ++i)
		_data[i] = _data[i] + rhs;
	return *this;
}

template <class T>
HVec<T>& HVec<T>::operator-=(const T& rhs) {
	for (size_t i = 0; i < _dims; ++i)
		_data[i] = _data[i] - rhs;
	return *this;
}

template <class T>
HVec<T>& HVec<T>::operator*=(const HVec<T>& rhs) {
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions must match for the *= to be computed.");
	for (size_t i = 0; i < _dims; ++i)
		_data[i] = _data[i] * rhs.At(i);
	return *this;
}

template <class T>
HVec<T>& HVec<T>::operator+=(const HVec<T>& rhs) {
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions must match for the += to be computed.");
	for (size_t i = 0; i < _dims; ++i)
		_data[i] = _data[i] + rhs.At(i);
	return *this;
}

template <class T>
HVec<T>& HVec<T>::operator-=(const HVec<T>& rhs) {
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions must match for the -= to be computed.");
	for (size_t i = 0; i < _dims; ++i)
		_data[i] = _data[i] - rhs.At(i);
	return *this;
}

//template<class T>
//inline Eigen::Matrix<T, Eigen::Dynamic, 1> HVec<T>::ToEigenVec()
//{
//	return Eigen::Matrix<T, _dims, 1>(_data);
//}
// ==========================================================

//  scalars ==================================================================

template <class T>
T HVec<T>::dot(const HVec<T>& a, const HVec<T>& b)
{
	if (a._dims != b._dims) throw std::invalid_argument("Vector dimensions must match for the dot-product to be computed.");

	T cumulativeSum = static_cast<T>(0.0);
	for (size_t i = 0; i < a._dims; ++i)
		cumulativeSum += a._data.at(i) * b._data.at(i);

	return cumulativeSum;
}

template <class T>
HVec<T> HVec<T>::cross(const HVec<T>& a, const HVec<T>& b)
{
	if (a._dims != b._dims) throw std::invalid_argument("Vector dimensions must match for the cross-product to be computed.");
	if (a._dims != 3) throw std::invalid_argument("The cross-product can only be computed for three-dimensional vectors."); // для семи- еще можно

	return HVec<T> {
		(a._data.at(1)* b._data.at(2)) - (a._data.at(2) * b._data.at(1)),
			-((a._data.at(0) * b._data.at(2)) - (a._data.at(2) * b._data.at(0))),
			(a._data.at(0)* b._data.at(1)) - (a._data.at(1) * b._data.at(0))
	};
}