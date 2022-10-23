#include "geometry/primitives3d/structers.h"
#include "geometry/funcs.h"

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