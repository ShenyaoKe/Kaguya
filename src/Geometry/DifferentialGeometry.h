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
#include "Math/CGVector.h"

/************************************************************************/
/* DifferentialGeometry                                                 */
/************************************************************************/
struct DifferentialGeometry
{
	const Shape* object;
	//int objID;
	mutable Point3D pos;
	mutable Vector3D normal, reflectDir;
	mutable Vector3D lightDir;
	mutable Float lightDist;
	//Sample related viriables
	int sIndexX, sIndexY, sample;
	int shiftX, shiftY, sampleOffset;

	mutable Point3D UV;
	mutable Vector3D dpdu, dpdv;//Partial derivation of the surface position, marked as dpdu, dpdv
	Vector3D dndu, dndv;//Partial derivation of the surface normal

	DifferentialGeometry();
	DifferentialGeometry(const Shape* obj, const Vector3D &pVec, const Vector3D &nVec, const Vector3D &rVec, const int& sX, const int& sY);

	void setObject(const Shape* obj);
	void setPos(const Vector3D &pVec);
	void setNormal(const Vector3D &nVec);
	void setReflectDir(const Vector3D &rVec);
 	void calculateDir(const Vector3D &inDir, const Vector3D &nVec);
 	void calculateDir(const Vector3D &inDir);
 	void setSample(const int& aaS, const int& aaOffset);
 	void setSampleIndex(const int& x, const int& y);
 	Float getDiffuseTheta() const;
 	Float getSpecularTheta() const;
};
#endif // __DifferentialGeometry__