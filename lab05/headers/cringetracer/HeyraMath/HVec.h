#pragma once

#include "../../geometry/primitives3d/structers.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include "Eigen/Eigen"

/*
	float x, y, z;
	ImVec3() : x(0.0f), y(0.0f), z(0.0f) { }
	constexpr ImVec3(const float& _x, const float& _y, const float& _z) : x(_x), y(_y), z(_z) { }
	//[[deprecated("I m not sure that rvalues are operated correctly")]]
	constexpr ImVec3(float&& _x, float&& _y, float&& _z) : x(_x), y(_y), z(_z) { }
	constexpr ImVec3(const float& _val) : x(_val), y(_val), z(_val) { }
	constexpr ImVec3(ImVec2 v2, float _z = 0.0) : x(v2.x), y(v2.y), z(_z) { }
	inline float  operator[] (size_t idx) const { assert(idx <= 2); return (&x)[idx]; }
	inline float& operator[] (size_t idx) { assert(idx <= 2); return (&x)[idx]; }

	ImVec3& operator+=(const ImVec3& rhs);
	ImVec3& operator/=(const ImVec3& rhs);
	ImVec3& operator/=(const float rhs);
	ImVec3& cross_product(const ImVec3& rhs);
*/
// i was curious about overloads. so here we have
template <class T> struct HVec
{
	T x, y, z;
	constexpr HVec() : x((T)0.0), y((T)0.0), z((T)0.0) { }
	constexpr HVec(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) { }

	HVec(const ImVec3& imvec) : x((T)imvec.x), y((T)imvec.y), z((T)imvec.z) { }

	inline T  operator[] (size_t idx) const { assert(idx <= 2); return (&x)[idx]; }
	inline T& operator[] (size_t idx) { assert(idx <= 2); return (&x)[idx]; }

	inline const T& At(const size_t index) const { return this->operator[](index); }
	inline void SetAt(const size_t index, T value) { this->operator[](index) = value; }

	inline T len() const { return std::sqrt(dot(*this, *this)); }
	HVec<T> Normalized() const {
		T vecNorm = 1.0 / this->len();
		return *this * vecNorm;
	};

	void Normalize() {
		T vecNorm = 1.0 / this->len();
		x *= vecNorm;
		y *= vecNorm;
		z *= vecNorm;
	};

	inline HVec<T> operator+ (const HVec<T>& rhs) const {
		return HVec<T>{ x + rhs.x, y + rhs.y, z + rhs.z, };
	};
	inline HVec<T> operator- (const HVec<T>& rhs) const {
		return HVec<T>{ x - rhs.x, y - rhs.y, z - rhs.z, };
	};
	inline HVec<T> operator* (const HVec<T>& rhs) const {
		return HVec<T>{ x* rhs.x, y* rhs.y, z* rhs.z, };
	};
	inline HVec<T> operator* (const T& rhs) const {
		return HVec<T>{ x* rhs, y* rhs, z* rhs, };
	};
	inline HVec<T>& operator*=(const T& rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	inline HVec<T>& operator+=(const T& rhs) {
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}
	inline HVec<T>& operator-=(const T& rhs) {
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	inline HVec<T>& operator*=(const HVec<T>& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	inline HVec<T>& operator+=(const HVec<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	inline HVec<T>& operator-=(const HVec<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

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

	inline static T dot(const HVec<T>& a, const HVec<T>& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	inline static HVec<T> cross(const HVec<T>& a, const HVec<T>& b) {
		return HVec<T> {
			(a.y* b.z) - (a.z * b.y),
				-((a.x * b.z) - (a.z * b.x)),
				(a.x* b.y) - (a.y * b.x)
		};
	}
};

// ===========================================

template <class T>
HVec<T> operator* (const T& lhs, const HVec<T>& rhs)
{
	return rhs * lhs;
}

template <class T> struct HVec2
{
	T x, y;
	constexpr HVec2() : x((T)0.0), y((T)0.0) { }
	constexpr HVec2(const T& _x, const T& _y) : x(_x), y(_y) { }

	HVec2(const ImVec2& imvec) : x((T)imvec.x), y((T)imvec.y) { }

	inline T  operator[] (size_t idx) const { assert(idx <= 1); return (&x)[idx]; }
	inline T& operator[] (size_t idx) { assert(idx <= 1); return (&x)[idx]; }

	inline const T& At(const size_t index) const { return this->operator[](index); }
	inline void SetAt(const size_t index, T value) { this->operator[](index) = value; }

	inline T len() const { return std::sqrt(HVec2<T>::dot(*this, *this)); }
	HVec2<T> Normalized() const {
		T vecNorm = 1.0 / this->len();
		return *this * vecNorm;
	};

	void Normalize() {
		T vecNorm = 1.0 / this->len();
		x *= vecNorm;
		y *= vecNorm;
	};

	inline HVec2<T> operator+ (const HVec2<T>& rhs) const {
		return HVec2<T>{ x + rhs.x, y + rhs.y };
	};
	inline HVec2<T> operator- (const HVec2<T>& rhs) const {
		return HVec2<T>{ x - rhs.x, y - rhs.y };
	};
	inline HVec2<T> operator* (const HVec2<T>& rhs) const {
		return HVec2<T>{ x* rhs.x, y* rhs.y };
	};
	inline HVec2<T> operator* (const T& rhs) const {
		return HVec2<T>{ x* rhs, y* rhs };
	};
	inline HVec2<T>& operator*=(const T& rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}
	inline HVec2<T>& operator+=(const T& rhs) {
		x += rhs;
		y += rhs;
		return *this;
	}
	inline HVec2<T>& operator-=(const T& rhs) {
		x -= rhs;
		y -= rhs;
		return *this;
	}

	inline HVec2<T>& operator*=(const HVec2<T>& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}
	inline HVec2<T>& operator+=(const HVec2<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	inline HVec2<T>& operator-=(const HVec2<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	template <class U> friend HVec2<U> operator* (const U& lhs, const HVec2<U>& rhs);

	inline static T dot(const HVec2<T>& a, const HVec2<T>& b) {
		return a.x * b.x + a.y * b.y;
	}
};

template <class T>
HVec2<T> operator* (const T& lhs, const HVec2<T>& rhs)
{
	return rhs * lhs;
}