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
	Point3D pos;// origin point position
	Vector3D dir;//Direction
	mutable Float tmin = 0.0, tmax = INFINITY;
	Float time;
	int dp;//ray depth count

	Ray();
	Ray(const Point3D& p, const Vector3D& d,
		double minT = 0.0, double maxT = INFINITY);
	~Ray();

	Point3D operator()(const Float& t) const;
	void setPos(Point3D& vp);
	void setDir(Vector3D& vd);
	void setT(const Float& t1, const Float& t2) const;
	Point3D getPos() const;
	Vector3D getDir() const;
	Float getDifferenceT() const;
	//void printDirLen() const{ cout << dir.getLenSq() << endl; }//
	//Vector3D getPoint(Float t) const;
	//Float getDist(Float t) const;
	void normalize();

	void printInfo() const;
};

#endif