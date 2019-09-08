//
//  Ray.h
//
//  Created by Shenyao Ke on 1/28/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#include "Math/MathUtil.h"
#include "Math/Vector.h"

namespace Kaguya
{

class Ray
{
public:
	Ray(const Point3f &pos = Point3f(0, 0, 0),
		const Vector3f &dir = Vector3f(1, 0, 0),
		Float minT = NUM_ZERO, Float maxT = sNumInfinity);

	Point3f operator()(Float t) const;
	void setT(Float t1, Float t2) const;
	Float getDifferenceT() const;

	void printInfo() const;

public:
	Point3f o;// Origin point position
	mutable Float tMin;

	Vector3f d;// Ray Direction
	Float time;

	mutable Float tMax;
	uint32_t mask;
	uint32_t mId; // Ray Id
	uint32_t mFlags;

	// Intersection Information
	// Unnormalized Geometry Normal
	Normal3f Ng;

	// Barycentric coordinates of intersection
	mutable Float u;
	mutable Float v;

	// Geometry ID
	uint32_t geomID;
	// Primitive ID
	uint32_t primID;
	// Instance ID
	uint32_t instID;
};

}
