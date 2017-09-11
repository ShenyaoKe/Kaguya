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
/* Intersection                                                 */
/************************************************************************/
class Intersection
{
public:
	Intersection() : shape(nullptr) {}
	Intersection(const Point3f &_p, const Normal3f &_n,
						 const Point2f &_uv, const Primitive* shp)
		: P(_p), Ng(_n), UV(_uv), shape(shp)
	{
	}
	Intersection(const Point3f &_p, const Normal3f &_n,
						 const Vector3f &_dpdu, const Vector3f &_dpdv,
						 const Normal3f &_dndu, const Normal3f &_dndv,
						 const Point2f &_uv, const Primitive* shp);

	void calculateDir(const Vector3f &inDir, const Normal3f &nVec);
	void calculateDir(const Vector3f &inDir);


	const Primitive* shape;
	Point3f          P;
	Normal3f         Ng;
	Point2f          UV;//barycentric coordinate

	Vector3f         dPdu, dPdv;//Partial derivation of the surface position
	Normal3f         dNdu, dNdv;//Partial derivation of the surface normal

	struct // Shading Geometry
	{
		Normal3f     N;
		Point2f      ST;
		Vector3f     dPds, dPdt;
		Normal3f     dNds, dNdt;
	} shading;
};

}
