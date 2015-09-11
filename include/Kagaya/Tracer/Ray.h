//
//  Ray.h
//
//  Created by Shenyao Ke on 1/28/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Ray__
#define __Ray__
#include "Math/CGVector.h"

class Ray
{
private:
	
	
public:
	Point3D pos;// origin point position
	Vector3D dir;//Direction
	mutable double tmin = 0, tmax = INFINITY;
	double time;
	int dp;//ray depth count

	Ray();
	virtual ~Ray();

	virtual Point3D operator()(const double& t) const;
	Ray(const Point3D& p, const Vector3D& d);
	void setPos(Point3D& vp);
	void setDir(Vector3D& vd);
	void setT(const double& t1, const double& t2) const;
	Point3D getPos() const;
	Vector3D getDir() const;
	double getDifferenceT() const;
	//void printDirLen() const{ cout << dir.getLenSq() << endl; }//
	//Vector3D getPoint(double t) const;
	//double getDist(double t) const;
	void normalize();

	void printInfo() const;
};

#endif