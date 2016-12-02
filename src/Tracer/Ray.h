//
//  Ray.h
//
//  Created by Shenyao Ke on 1/28/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Ray__
#define __Ray__

#include "Math/MathUtil.h"
#include "Math/Geometry.h"

class Ray
{
private:

public:
	Ray();
	Ray(const Point3f &pos, const Vector3f &dir,
		Float minT = 0, Float maxT = NUM_INFINITY);

	Point3f operator()(Float t) const;
	void setT(Float t1, Float t2) const;
	Float getDifferenceT() const;

	void printInfo() const;
public:
	Point3f o;// Origin point position
	Vector3f d;// Ray Direction
	mutable Float u, v;
	mutable Float tmin = 0.0, tmax = NUM_INFINITY;
	Float time;
	int dp;//ray depth count
	int32_t geomID;
	int32_t primID;
};

#endif