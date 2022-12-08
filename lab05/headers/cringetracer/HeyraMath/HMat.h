#pragma once
#include <vector>
#include <stdexcept>
#include "HVec.h"

template <class T> class HMat
{
	size_t _rows, _cols, _size;
	T* _data;

	size_t Sub2Ind(const size_t row, const size_t col) const;
public:
	HMat();
	HMat(const size_t inRows, const size_t inCols);
	HMat(const size_t inRows, const size_t inCols, const T* inputData);
	HMat(const HMat<T>& inputMat);
	HMat(const size_t inRows, const size_t inCols, const std::vector<T>& inputData);

	~HMat();

	bool IsSquare();
	bool Resize(const size_t inRows, const size_t inCols);
	bool Inverse();
	HMat<T> Transpose() const;

	// ========================================================

	const T& At(const size_t inRows, const size_t inCols) const;
	bool SetAt(const size_t inRows, const size_t inCols, const T& elementValue);
	size_t Rows() const;
	size_t Cols() const;
	size_t Size() const;
	// ========================================================

	template <class U> friend HMat<U> operator+ (const HMat<U>& lhs, const HMat<U>& rhs);
	template <class U> friend HMat<U> operator+ (const U& lhs, const HMat<U>& rhs);
	template <class U> friend HMat<U> operator+ (const HMat<U>& lhs, const U& rhs);

	template <class U> friend HMat<U> operator- (const HMat<U>& lhs, const HMat<U>& rhs);
	template <class U> friend HMat<U> operator- (const U& lhs, const HMat<U>& rhs);
	template <class U> friend HMat<U> operator- (const HMat<U>& lhs, const U& rhs);

	template <class U> friend HMat<U> operator* (const HMat<U>& lhs, const HMat<U>& rhs);
	template <class U> friend HMat<U> operator* (const U& lhs, const HMat<U>& rhs);
	template <class U> friend HMat<U> operator* (const HMat<U>& lhs, const U& rhs);

	template <class U> friend HVec<U> operator* (const HMat<U>& lhs, const HVec<U>& rhs);

};

template <class T>
bool HMat<T>::IsSquare()
{
	if (_cols == _rows) return true;
	return false;
}

template<class T>
inline size_t HMat<T>::Sub2Ind(const size_t row, const size_t col) const
{
	if ((row < _rows) && (row >= 0) && (col < _cols) && (col >= 0))
		return (row * _cols) + col;

	throw std::out_of_range("row, col -> linear ind");
}

template <class T> HMat<T>::HMat()
{
	_rows = 0;
	_cols = 0;
	_size = 0;
	_data = nullptr;
}

template<class T>
inline HMat<T>::HMat(const size_t inRows, const size_t inCols)
{
	_rows = inRows;
	_cols = inCols;
	_size = inRows * inCols;
	_data = new T[_size];
	for (size_t i = 0; i < _size; i++) _data[i] = (T)0;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

template<class T>
inline HMat<T>::HMat(const size_t inRows, const size_t inCols, const T* inputData)
{
	_rows = inRows;
	_cols = inCols;
	_size = _rows * _cols;
	_data = new T[_size];
	for (int i = 0; i < _size; i++)
		_data[i] = inputData[i];
}

template<class T>
inline HMat<T>::HMat(const HMat<T>& inputMat)
{
	_rows = inputMat._rows;
	_cols = inputMat._cols;
	_size = inputMat._size;

	_data = new T[_size];
	for (int i = 0; i < _size; i++)
		_data[i] = inputMat._data[i];
}

template<class T>
inline HMat<T>::HMat(const size_t inRows, const size_t inCols, const std::vector<T>& inputData)
{
	_rows_rows = inRows;
	_cols = inCols;
	_size = _rows * _cols;
	_data = new T[_size];
	for (int i = 0; i < _size; ++i)
		_data[i] = inputData.at(i);
}

template <class T> HMat<T>::~HMat()
{
	if (_data) delete[] _data;
	_data = nullptr;
}

template<class T>
inline bool HMat<T>::Resize(const size_t inRows, const size_t inCols)
{
	_rows = inRows;
	_cols = inCols;
	_size = (_rows * _cols);
	delete[] _data;
	_data = new T[_size];
	if (_data != nullptr)
	{
		for (int i = 0; i < _size; i++)
			_data[i] = 0.0;

		return true;
	}
	return false;
}

template<class T>
inline bool HMat<T>::Inverse()
{
	return false;
}

template<class T>
inline HMat<T> HMat<T>::Transpose() const
{
	return HMat<T>();
}

// ========================================================

template<class T>
inline const T& HMat<T>::At(const size_t inRows, const size_t inCols) const
{
	return T();
}

template<class T>
inline bool HMat<T>::SetAt(const size_t inRows, const size_t inCols, const T& elementValue)
{
	return false;
}

template<class T>
inline size_t HMat<T>::Rows() const
{
	return _rows;
}

template<class T>
inline size_t HMat<T>::Cols() const
{
	return _cols;
}

template<class T>
inline size_t HMat<T>::Size() const
{
	return _size;
}

// ========================================================

template<class U>
inline HMat<U> operator+(const HMat<U>& lhs, const HMat<U>& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator+(const U& lhs, const HMat<U>& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator+(const HMat<U>& lhs, const U& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator-(const HMat<U>& lhs, const HMat<U>& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator-(const U& lhs, const HMat<U>& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator-(const HMat<U>& lhs, const U& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator*(const HMat<U>& lhs, const HMat<U>& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator*(const U& lhs, const HMat<U>& rhs)
{
	return HMat<U>();
}

template<class U>
inline HMat<U> operator*(const HMat<U>& lhs, const U& rhs)
{
	return HMat<U>();
}

template<class U>
inline HVec<U> operator*(const HMat<U>& lhs, const HVec<U>& rhs)
{
	return HVec<U>();
}
