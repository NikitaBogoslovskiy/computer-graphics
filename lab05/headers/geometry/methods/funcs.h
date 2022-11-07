#pragma warning( disable : 4244 )
#ifndef _FUNCS_H_
#define _FUNCS_H_

#include "pch.h"
#include <algorithm>    // std::min
#include "geometry/primitives2d/point.h"
//#include "geometry/primitives3d/mesh.h"
#include <set>

#define PI 3.14159265359f

struct edgePointer { // This structure is named "myDataType"
	ImVec2* p1;
	ImVec2* p2;
};

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline float operator*(const ImVec2& lhs, const ImVec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
inline ImVec2 operator*(const float& lhs, const ImVec2& rhs) { return ImVec2(lhs * rhs.x, lhs * rhs.y); }
inline bool operator==(const ImVec2& lhs, const ImVec2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
inline float operator*(const ImVec4& lhs, const ImVec4& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }
inline ImVec4 operator*(const float& lhs, const ImVec4& rhs) { return ImVec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }


inline ImU32 GetColorV4U32(const ImVec4& curr_color) {
	return IM_COL32(curr_color.x, curr_color.y, curr_color.z, curr_color.w);
}

inline ImU32 GetColorFlU32(const float* curr_color) {
	return (IM_COL32((int)(curr_color[0] * 255), (int)(curr_color[1] * 255), (int)(curr_color[2] * 255), (int)(curr_color[3] * 255)));
}

inline ImVec4 GetColorFlV4(const float* curr_color) {
	return ImVec4((int)(curr_color[0] * 255), (int)(curr_color[1] * 255),(int)(curr_color[2] * 255), (int)(curr_color[3] * 255));
}

inline float DegreesToRadians(const float& degrees) {
	return degrees * (PI / 180);
}

template<typename _Container,
	typename _Value = typename _Container::value_type,
	typename = std::enable_if_t<std::is_convertible_v<_Value, Primitive*>>>
	std::tuple<int, ImVec2*> detect_point(const _Container& primitives) {
	auto originIt = std::find_if(primitives.begin(), primitives.end(), [](const auto& prim) { return dynamic_cast<Point*>(prim) != NULL; });
	if (originIt == primitives.end()) return std::make_tuple(0, nullptr);
	return std::make_tuple(1, &(dynamic_cast<Point*>(*originIt)->at(0)));
}

template<typename _Container,
	typename _Value = typename _Container::value_type,
	typename = std::enable_if_t<std::is_convertible_v<_Value, Primitive*>>>
	int tr_chpr_rtp(const _Container& primitives, std::function<void(Primitive*, ImVec2*)> lammy) {
	if (primitives.size() == 0) throw std::invalid_argument("No primitives picked");
	int pointsCount; ImVec2* origin;
	std::tie(pointsCount, origin) = detect_point(primitives);

	std::for_each(primitives.begin(), primitives.end(), [&lammy, &origin](Primitive* prim) {
		auto test = dynamic_cast<Point*>(prim);
		if (test != NULL && &(test->at(0)) == origin) return;
		lammy(prim, origin);
		});
	return 0;
}

Primitive midpointDisplacement(Primitive& displacement, Point* p1, Point* p2, int R, int I, int iter_num);
Primitive* packPresent(Point* bottom_point, const std::set<Primitive*>& chosen_prims, const ImU32& color, const float& thickness);
ImVector<ImVec2>* unionPolygons(Primitive* p1, Primitive* p2);
/*
Mesh* rotationBodyX(std::vector<Point>& points, int iterNumber);
Mesh* rotationBodyY(std::vector<Point>& points, int iterNumber);
Mesh* rotationBodyZ(std::vector<Point>& points, int iterNumber);
Mesh* rotationBody(std::vector<Point>& points, int iterNumber, Eigen::Matrix<float, 4, 4>& rmat);
*/

static float distance(const ImVec2& a, const ImVec2& b) {
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}
static float distance_from_line(const ImVec2& e1, const ImVec2& e2, const ImVec2& point) {
	float left = distance(e1, point),
		base = distance(e1, e2),
		right = distance(e2, point);
	float p = 0.5 * (left + base + right);
	float S = sqrtf(p * (p - left) * (p - base) * (p - right));
	return 2 * S / base;
}

#include "geometry/primitives3d/structers.h"

inline ImVec3 operator+(const ImVec3& lhs, const ImVec3& rhs) { return ImVec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
inline ImVec3 operator-(const ImVec3& lhs, const ImVec3& rhs) { return ImVec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
inline float operator*(const ImVec3& lhs, const ImVec3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
inline ImVec3 operator*(const float& lhs, const ImVec3& rhs) { return ImVec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
inline ImVec3 operator*(const ImVec3& lhs, const float& rhs) { return rhs * lhs; }
inline bool operator==(const ImVec3& lhs, const ImVec3& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
inline ImVec3 operator/(const ImVec3& lhs, const float& rhs) { return ImVec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }

// sometime in future, but now just this
template<class C>
static C sum(std::vector<C> vT) {
	C t = C();
	for (auto& vt : vT) {
		t += vt;
	}
	return t;
}

#endif