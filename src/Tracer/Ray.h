//
//  Ray.h
//
//  Created by Shenyao Ke on 1/28/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Ray__
#define __Ray__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Math/Geometry.h"

class Ray
{
private:

public:
	Ray();
	Ray(const Point3f &pos, const Vector3f &dir,
		double minT = 0.0, double maxT = INFINITY);

	Point3f operator()(const Float& t) const;
	void setT(const Float& t1, const Float& t2) const;
	Float getDifferenceT() const;

	void printInfo() const;
public:
	Point3f o;// Origin point position
	Vector3f d;// Ray Direction
	mutable Float tmin = 0.0, tmax = INFINITY;
	Float time;
	int dp;//ray depth count
};

#endif