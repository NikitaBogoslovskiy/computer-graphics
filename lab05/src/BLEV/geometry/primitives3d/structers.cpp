#include "geometry/primitives3d/structers.h"
#include "geometry/methods/funcs.h"

ImVec3 Polygon::center(const std::vector<ImVec3>& points)
{
    ImVec3 ret;
    for (auto idx : indices) {
        ret += points[idx];
    }
    return ret / indices.size();
}

ImVec3& ImVec3::operator+=(const ImVec3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

ImVec3& ImVec3::operator/=(const ImVec3& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

ImVec3& ImVec3::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

ImVec3& ImVec3::cross_product(const ImVec3& rhs)
{
    auto _x = y * rhs.z + z * rhs.y;
    auto _y = z * rhs.x + x * rhs.z;
    auto _z = x * rhs.y + y * rhs.x;
    x = _x;
    y = _y;
    z = _z;
    return *this;
}
