//
//  Intersection.h
//
//  Created by Shenyao Ke on 3/1/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#include "Core/Kaguya.h"
#include "Math/Vector.h"

namespace Kaguya
{

/************************************************************************/
/* Intersection                                                         */
/************************************************************************/
class Intersection
{
public:
	Intersection() : mShape(nullptr) {}
	Intersection(const Point3f &p, const Normal3f &n,
				 const Point2f &uv, const Geometry* shp)
		: mShape(shp), mPos(p), mGeomN(n), mUV(uv)
	{
	}
	Intersection(const Point3f &p, const Normal3f &n,
				 const Vector3f &dpdu, const Vector3f &dpdv,
				 const Normal3f &dndu, const Normal3f &dndv,
				 const Point2f &uv, const Geometry* shp);

	void calculateDir(const Vector3f &inDir, const Normal3f &nVec);
	void calculateDir(const Vector3f &inDir);

public:
	const Geometry* mShape;
	Point3f         mPos;
	Normal3f        mGeomN;
	//barycentric coordinate
	Point2f         mUV;
	//Partial derivation of the surface position, dPdu and dPdv
	Vector3f        mPu, mPv;
	//Partial derivation of the surface normal, dNdu and dNdv
	Normal3f        mNu, mNv;

	// Shading Geometry
	Normal3f        mShadingN;
	//Texture Coordinates
	Point2f         mST;
	// dPds, dPdt
	Vector3f        mPs, mPt;
	// dNds, dNdt
	Normal3f        mNs, mNt;
};

}
