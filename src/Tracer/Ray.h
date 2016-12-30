//
//  Ray.h
//
//  Created by Shenyao Ke on 1/28/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#include "Math/MathUtil.h"
#include "Math/Geometry.h"

namespace Kaguya
{

class Ray
{
public:
    Ray(const Point3f &pos = Point3f(0, 0, 0),
        const Vector3f &dir = Vector3f(1, 0, 0),
        Float minT = 0, Float maxT = NUM_INFINITY);

    Point3f operator()(Float t) const;
    void setT(Float t1, Float t2) const;
    Float getDifferenceT() const;

    void printInfo() const;

public:
    Point3f o;// Origin point position
    float   align0;

    Vector3f d;// Ray Direction
    float   align1;

    mutable Float tmin = 0;
    mutable Float tmax = NUM_INFINITY;

    Float time;
    uint32_t mask;

    // Unnormalized Geometry Normal
    Normal3f Ng;
    float align2;

    // Barycentric coordinates of intersection
    mutable Float u, v;

    // Geometry ID
    uint32_t geomID;
    // Primitive ID
    uint32_t primID;
    // Instance ID
    uint32_t instID;

    uint32_t dp;//ray depth count

    float align3[2];
};

}
