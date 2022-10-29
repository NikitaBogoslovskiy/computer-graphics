#pragma once

// #define NDEBUG
#include <cassert>
#include <deque>
#include "pch.h"

struct ImVec3
{
    float x, y, z;
    constexpr ImVec3() : x(0.0f), y(0.0f), z(0.0f) { }
    constexpr ImVec3(const float& _x, const float& _y, const float& _z) : x(_x), y(_y), z(_z) { }
    //[[deprecated("I m not sure that rvalues are operated correctly")]]
    constexpr ImVec3(float&& _x, float&& _y, float&& _z) : x(_x), y(_y), z(_z) { }
    constexpr ImVec3(const float& _val) : x(_val), y(_val), z(_val) { }
    constexpr ImVec3(ImVec2 v2, float _z = 0.0) : x(v2.x), y(v2.y), z(_z) { }
    inline float  operator[] (size_t idx) const { assert(idx <= 2); return (&x)[idx]; }
    inline float& operator[] (size_t idx) { assert(idx <= 2); return (&x)[idx]; }

    ImVec3& operator+=(const ImVec3& rhs);
};

struct Polygon
{
    Polygon() = default;
    std::deque<uint32_t> indices;
    Polygon(const std::initializer_list<uint32_t>& _indices) : indices(_indices) { assert(_indices.size() >= 3); }
    Polygon(std::initializer_list<uint32_t>&& _indices) : indices(_indices) { assert(indices.size() >= 3); }
    Polygon(const Polygon& _polygon) noexcept : indices(_polygon.indices) {}
    Polygon(Polygon&& _polygon) noexcept : indices(std::move(_polygon.indices)) {}
    inline uint32_t  operator[] (size_t idx) const { return indices[idx]; }
    inline uint32_t& operator[] (size_t idx) { return indices[idx]; }
    inline uint32_t& at(size_t idx) { return indices[idx]; }
    inline size_t size() const noexcept { return indices.size(); }
    inline void push_back(uint32_t val) { indices.push_back(val); }
    inline void pop_back() { assert(indices.size() > 3); indices.pop_back(); } 
    inline void push_front(uint32_t val) { indices.push_front(val); }
    inline void pop_front() { assert(indices.size() > 3); indices.pop_front(); }
    inline void insert(size_t idx, uint32_t val) { assert(indices.size() > idx);  indices.insert(indices.begin() + idx, val); }
    ImVec3 center(const std::vector<ImVec3>& points);
    
    void draw(ImDrawList* draw_list, const ImVec2& offset, Eigen::Matrix4f& vp) {
        
    }
};

struct VisualParams {
    ImU32 color;
    float thickness;
    bool show;
    constexpr VisualParams() noexcept : color(IM_COL32(255, 255, 204, 255)), thickness(1.f), show(true) {};
    VisualParams(const ImU32& _color, const float& _thickness, bool _show) : color(_color), thickness(_thickness), show(_show) {}
    VisualParams(ImU32&& _color, float&& _thickness, bool&& _show) : color(std::move(_color)), thickness(std::move(_thickness)), show(std::move(_show)) {}
    VisualParams(const VisualParams& vp) : color(vp.color), thickness(vp.thickness), show(vp.show) {};
    VisualParams(VisualParams&& vp) noexcept : color(std::move(vp.color)), thickness(std::move(vp.thickness)), show(std::move(vp.show)) {};
    virtual void draw(ImDrawList* draw_list, const ImVec2& offset, Eigen::Matrix4f vp) {}
};

#include "geometry/methods/funcs.h"

struct Point3d : public VisualParams {
    ImVec3 point;
    Point3d() : point() {}
    Point3d(const ImVec3& _point) : point(_point) {}
    Point3d(ImVec3&& _point) : point(std::move(_point)) {}

    virtual void draw(ImDrawList* draw_list, const ImVec2& offset, Eigen::Matrix4f vp) override {
        if (show) {
            Eigen::Vector4f v0{ point.x, point.y, point.z,  1.f }; // COLUMN-VEC
            Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
            auto start = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3));
            draw_list->AddCircleFilled(start + offset, thickness, color, 10);
        }
    }
};

struct Line3d : public VisualParams {
    ImVec3 p1;
    ImVec3 p2;
    bool infinity;
    Line3d() : p1(), p2(), infinity(false) {}
    Line3d(const ImVec3& _p1, const ImVec3& _p2, bool _inf = false) : p1(_p1), p2(_p2), infinity(_inf) {}
    Line3d(ImVec3&& _p1, ImVec3&& _p2, bool _inf = false) : p1(std::move(_p1)), p2(std::move(_p2)), infinity(_inf) {}
    virtual void draw(ImDrawList* draw_list, const ImVec2& offset, Eigen::Matrix4f vp) override {
        if (show) {
            Eigen::Vector4f v0{ p1.x, p1.y, p1.z,  1.f }; // COLUMN-VEC
            Eigen::Vector4f v1{ p2.x, p2.y, p2.z,  1.f };

            Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
            Eigen::Vector4f v1_2d = vp * v1;

            auto start = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3));
            auto end = ImVec2(v1_2d(0) / v1_2d(3), v1_2d(1) / v1_2d(3));

            //start.x *= 512.f * 0.5f; start.y *= 512.f * 0.5f; //if we use fov perspective
            //end.x *= 512.f * 0.5f; end.y *= 512.f * 0.5f;
            draw_list->AddLine(start + offset, end + offset, color, thickness);
        }
    }

    static void draw(ImDrawList* draw_list, const ImVec3& p1, const ImVec3& p2, const ImVec2& offset, Eigen::Matrix4f vp, VisualParams _vp) {
        Eigen::Vector4f v0{ p1.x, p1.y, p1.z,  1.f }; // COLUMN-VEC
        Eigen::Vector4f v1{ p2.x, p2.y, p2.z,  1.f };

        Eigen::Vector4f v0_2d = vp * v0;// thus we projected v0 onto 2d canvas
        Eigen::Vector4f v1_2d = vp * v1;

        auto start = ImVec2(v0_2d(0) / v0_2d(3), v0_2d(1) / v0_2d(3));
        auto end = ImVec2(v1_2d(0) / v1_2d(3), v1_2d(1) / v1_2d(3));

        //start.x *= 512.f * 0.5f; start.y *= 512.f * 0.5f; //if we use fov perspective
        //end.x *= 512.f * 0.5f; end.y *= 512.f * 0.5f;
        draw_list->AddLine(start + offset , end + offset, _vp.color, _vp.thickness);
    }
};

//typedef Eigen::Product<Eigen::Matrix4f, Eigen::Matrix4f, 0> Matrix4fProduct;