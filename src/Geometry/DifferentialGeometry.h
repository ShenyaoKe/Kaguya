//
//  DifferentialGeometry.h
//
//  Created by Shenyao Ke on 3/1/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __DifferentialGeometry__
#define __DifferentialGeometry__

#include "Core/Kaguya.h"
#include "Math/Geometry.h"

/************************************************************************/
/* DifferentialGeometry                                                 */
/************************************************************************/
class DifferentialGeometry
{
public:
	DifferentialGeometry() : shape(nullptr) {}
	DifferentialGeometry(
		const Point3f &_p, const Normal3f &_n,
		const Vector2f &_st, const Shape* shp)
		: P(_p), Ng(_n), st(_st), shape(shp) {}
	DifferentialGeometry(const Point3f &_p, const Normal3f &_n,
		const Vector3f &_dpdu, const Vector3f &_dpdv,
		const Normal3f &_dndu, const Normal3f &_dndv,
		const Point2f &_uv, const Shape* shp);

 	void calculateDir(const Vector3f &inDir, const Normal3f &nVec);
 	void calculateDir(const Vector3f &inDir);


	const Shape* shape;
	Point3f P;
	Normal3f Ng;
	Vector2f st;//barycentric coordinate

	Point2f uv;
	Vector3f dPdu, dPdv;//Partial derivation of the surface position
	Normal3f dNdu, dNdv;//Partial derivation of the surface normal

	struct // Shading Geometry
	{
		Normal3f n;
		Vector3f dPdu, dPdv;
		Normal3f dNdu, dNdv;
	}shading;
};
#endif // __DifferentialGeometry__