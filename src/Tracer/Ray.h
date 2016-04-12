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

#include "Math/CGVector.h"

class Ray
{
private:

public:
	Ray();
	Ray(const Vector3D &pos, const Vector3D &dir,
		double minT = 0.0, double maxT = INFINITY);
	~Ray();

	Point3D operator()(const Float& t) const;
	void setPos(Vector3D &vp);
	void setDir(Vector3D &vd);
	void setT(const Float& t1, const Float& t2) const;
	Point3D getPos() const;
	Vector3D getDir() const;
	Float getDifferenceT() const;
	//void printDirLen() const{ cout << dir.getLenSq() << endl; }//
	//Vector3D getPoint(Float t) const;
	//Float getDist(Float t) const;
	void normalize();

	void printInfo() const;
public:
	Point3D o;// Origin point position
	Vector3D d;// Ray Direction
	mutable Float tmin = 0.0, tmax = INFINITY;
	Float time;
	int dp;//ray depth count
};

#endif