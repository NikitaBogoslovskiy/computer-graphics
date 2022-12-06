#pragma once

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

template <class T> class HVec
{
	std::vector<T> _data;
	size_t _dims;

public:
	HVec();
	~HVec();

	HVec(size_t numDims);
	HVec(const std::vector<T>& inputData);

	size_t GetDimensions() const;

	T At(const size_t index) const;
	void SetAt(const size_t index, T value);

	T len();
	HVec<T> Normalized();
	void Normalize();

	HVec<T> operator+ (const HVec<T>& rhs) const;
	HVec<T> operator- (const HVec<T>& rhs) const;
	HVec<T> operator* (const T& rhs) const;

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

template <class T> HVec<T>::HVec(const std::vector<T>& inputData)
{
	_dims = inputData.size();
	_data = inputData;
}

// =========================================== properties

template <class T>
size_t HVec<T>::GetDimensions() const
{
	return _dims;
}

template <class T>
T HVec<T>::At(size_t index) const
{
	return _data[index];
}

template <class T>
void HVec<T>::SetAt(const size_t index, T value)
{
	_data[index] = value;
}

template <class T>
T HVec<T>::len()
{
	T cumulativeSum = static_cast<T>(0.0);
	for (int i = 0; i < _dims; ++i)
		cumulativeSum += (_data.at(i) * _data.at(i));

	return sqrt(cumulativeSum);
}

template <class T>
HVec<T> HVec<T>::Normalized()
{
	T vecNorm = this->len();
	HVec<T> result(_data);
	return result * (static_cast<T>(1.0) / vecNorm);
}

template <class T>
void HVec<T>::Normalize()
{
	T vecNorm = this->len();
	for (int i = 0; i < _dims; ++i)
	{
		T temp = _data.at(i) * (static_cast<T>(1.0) / vecNorm);
		_data.at(i) = temp;
	}
}

template <class T>
HVec<T> HVec<T>::operator+ (const HVec<T>& rhs) const
{
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions do not match.");

	std::vector<T> resultData;
	for (int i = 0; i < _dims; ++i)
		resultData.push_back(_data.at(i) + rhs._data.at(i));

	HVec<T> result(resultData);
	return result;
}

template <class T>
HVec<T> HVec<T>::operator- (const HVec<T>& rhs) const
{
	if (_dims != rhs._dims) throw std::invalid_argument("Vector dimensions do not match.");

	std::vector<T> resultData(_dims);
	for (int i = 0; i < _dims; ++i)
		resultData[i] = _data.at(i) - rhs._data.at(i);

	HVec<T> result(resultData);
	return result;
}

template <class T>
HVec<T> HVec<T>::operator* (const T& rhs) const
{
	std::vector<T> resultData;
	for (int i = 0; i < _dims; ++i)
		resultData.push_back(_data.at(i) * rhs);

	HVec<T> result(resultData);
	return result;
}

template <class T>
HVec<T> operator* (const T& lhs, const HVec<T>& rhs)
{
	std::vector<T> resultData;
	for (int i = 0; i < rhs._dims; ++i)
		resultData.push_back(lhs * rhs._data.at(i));

	HVec<T> result(resultData);
	return result;
}

template <class T>
T HVec<T>::dot(const HVec<T>& a, const HVec<T>& b)
{
	if (a._dims != b._dims) throw std::invalid_argument("Vector dimensions must match for the dot-product to be computed.");

	T cumulativeSum = static_cast<T>(0.0);
	for (int i = 0; i < a._dims; ++i)
		cumulativeSum += a._data.at(i) * b._data.at(i);

	return cumulativeSum;
}

template <class T>
HVec<T> HVec<T>::cross(const HVec<T>& a, const HVec<T>& b)
{
	if (a._dims != b._dims) throw std::invalid_argument("Vector dimensions must match for the cross-product to be computed.");

	if (a._dims != 3) throw std::invalid_argument("The cross-product can only be computed for three-dimensional vectors.");

	std::vector<T> resultData;
	resultData.push_back((a._data.at(1) * b._data.at(2)) - (a._data.at(2) * b._data.at(1)));
	resultData.push_back(-((a._data.at(0) * b._data.at(2)) - (a._data.at(2) * b._data.at(0))));
	resultData.push_back((a._data.at(0) * b._data.at(1)) - (a._data.at(1) * b._data.at(0)));

	HVec<T> result(resultData);
	return result;
}