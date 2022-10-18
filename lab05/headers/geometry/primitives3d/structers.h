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
    [[deprecated("I m not sure that rvalues are operated correctly")]]
    constexpr ImVec3(float&& _x, float&& _y, float&& _z) : x(_x), y(_y), z(_z) { }
    constexpr ImVec3(ImVec2 v2, float _z = 0.0) : x(v2.x), y(v2.y), z(_z) { }
    inline float  operator[] (size_t idx) const { assert(idx <= 2); return (&x)[idx]; }
    inline float& operator[] (size_t idx) { assert(idx <= 2); return (&x)[idx]; }

    ImVec3& operator+=(const ImVec3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
};

struct Polygon
{
    std::deque<uint16_t> indices;
    Polygon(std::initializer_list<uint16_t> _indices) : indices(_indices) { assert(_indices.size() >= 3); }
    Polygon(std::initializer_list<uint16_t>&& _indices) : indices(_indices) { assert(indices.size() >= 3); }
    Polygon(const Polygon& _facet) noexcept : indices(_facet.indices) {}
    Polygon(Polygon&& _facet) noexcept : indices(std::move(_facet.indices)) {}
    inline uint16_t  operator[] (size_t idx) const { return indices[idx]; }
    inline uint16_t& operator[] (size_t idx) { return indices[idx]; }
    inline size_t size() const noexcept { return indices.size(); }
    inline void push_back(uint16_t val) { indices.push_back(val); }
    inline void pop_back() { assert(indices.size() > 3); indices.pop_back(); } 
    inline void push_front(uint16_t val) { indices.push_front(val); }
    inline void pop_front() { assert(indices.size() > 3); indices.pop_front(); }
    inline void insert(size_t idx, uint16_t val) { assert(indices.size() > idx);  indices.insert(indices.begin() + idx, val); }

    void draw(ImDrawList* draw_list) {

    }
};