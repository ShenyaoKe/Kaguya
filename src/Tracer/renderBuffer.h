//
//  renderBuffer.h
//
//  Created by Shenyao Ke on 2/13/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#include "Core/Kaguya.h"
#include "Math/Geometry.h"
#include "Geometry/Primitive.h"
#include "Geometry/DifferentialGeometry.h"

namespace Kaguya
{

class renderBuffer
{
public:
    renderBuffer(uint32_t w, uint32_t h);
    ~renderBuffer() {}

    void resize(uint32_t w, uint32_t h);
    void clear();
    bool empty() const;
    void setBuffer(uint32_t x, uint32_t y,
                   const DifferentialGeometry &geom, Float zdepth);

private:
    template<typename T>
    void Vec3ToFloats(const T &n, floats_t &buffer, size_t id)
    {
        buffer[id] = static_cast<float>(n.x);
        buffer[id + 1] = static_cast<float>(n.y);
        buffer[id + 2] = static_cast<float>(n.z);
    };
    template<typename T>
    void Vec2ToFloats(const T &n, floats_t &buffer, size_t id)
    {
        buffer[id] = static_cast<float>(n.x);
        buffer[id + 1] = static_cast<float>(n.y);
    };

public:
    uint32_t width;
    uint32_t height;
    size_t size;

    floats_t beauty;
    floats_t diff, spec;
    //floats_t fl, fr, sss;

    floats_t p, n;// 3 * n
    floats_t dpdu, dpdv, dndu, dndv; // 3 * n

    floats_t uv;// 2 * n

    floats_t z;// 1 * n
    ui32s_t id;// 1 * n

};

}
