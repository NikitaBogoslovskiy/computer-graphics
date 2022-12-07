#include "../headers/geometry/primitives3d/sphere.h"

std::pair<float, ImVec3> Sphere::is_intersected_with_ray(const ImVec3& sp, const ImVec3& direction, bool use_normals) const
{
    std::pair<float, ImVec3> ret = { FLT_MAX, 0 };

    //float a = dot_product(direction, direction);
    float b = 2.0f * dot_product(sp, direction);
    float c = dot_product(sp, sp) - powf(radius, 2.f);

    float discriminant = powf(b, 2.f) - 4.0f * 1.f * c;

    if (discriminant < 0.0f)
        return ret;
    float t0 = (-b + sqrtf(discriminant)) / 2.0f;
    float closestT = (-b - sqrtf(discriminant)) / 2.0f;
    //if (closestT < 0.f) return ret;

    return ret = { closestT, normilize(sp + closestT * direction - pos) };
}

bool Sphere::is_intersected_with_light(const ImVec3& sp, const ImVec3& direction, bool use_normals, float max_t) const
{
    //float a = dot_product(direction, direction);
    float b = 2.0f * dot_product(sp, direction);
    float c = dot_product(sp, sp) - powf(radius, 2.f);

    float discriminant = powf(b, 2.f) - 4.0f * 1.f * c;

    if (discriminant < 0.0f)
        return false;
    //float t0 = (-b + sqrtf(discriminant)) / 2.0f;
    
    float closestT = (-b - sqrtf(discriminant)) / 2.0f;
    
    //if (closestT < 0.f) return false;
    
    return closestT < max_t;
}