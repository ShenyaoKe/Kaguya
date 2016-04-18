//
//  DifferentialGeometry.h
//
//  Created by Shenyao Ke on 3/1/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __DifferentialGeometry__
#define __DifferentialGeometry__

#define KAGUYA_DOUBLE_AS_FLOAT

#include "Core/Kaguya.h"
//#include "Math/CGVector.h"
#include "Math/Geometry.h"

/************************************************************************/
/* DifferentialGeometry                                                 */
/************************************************************************/
struct DifferentialGeometry
{
	DifferentialGeometry();
	DifferentialGeometry(const Shape* obj, const Point3f &pVec, const Normal3f &nVec, const Vector3f &rVec, const int& sX, const int& sY);

 	void calculateDir(const Vector3f &inDir, const Normal3f &nVec);
 	void calculateDir(const Vector3f &inDir);
 	void setSample(const int& aaS, const int& aaOffset);
 	void setSampleIndex(const int& x, const int& y);


	const Shape* object;
	//int objID;
	Point3f pos;
	Normal3f normal;
	Vector3f reflectDir;
	//Sample related viriables
	int sIndexX, sIndexY, sample;
	int shiftX, shiftY, sampleOffset;

	Point2f UV;
	Vector3f dpdu, dpdv;//Partial derivation of the surface position, marked as dpdu, dpdv
	Vector3f dndu, dndv;//Partial derivation of the surface normal
};
#endif // __DifferentialGeometry__