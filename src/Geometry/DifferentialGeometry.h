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
#include "Math/Geometry.h"

/************************************************************************/
/* DifferentialGeometry                                                 */
/************************************************************************/
struct DifferentialGeometry
{
	DifferentialGeometry();
	DifferentialGeometry(const Shape* obj,
		const Point3f &pVec, const Normal3f &nVec,
		const int& sX, const int& sY);
	DifferentialGeometry(const Point3f &_p,
		const Vector3f &_dpdu, const Vector3f &_dpdv,
		const Normal3f &_dndu, const Normal3f &_dndv,
		const Point2f &_uv, const Shape* shp);

 	void calculateDir(const Vector3f &inDir, const Normal3f &nVec);
 	void calculateDir(const Vector3f &inDir);


	const Shape* shape;
	Point3f pos;
	Normal3f norm;

	Point2f uv;
	Vector3f dpdu, dpdv;//Partial derivation of the surface position
	Normal3f dndu, dndv;//Partial derivation of the surface normal

	struct // Shading Geometry
	{
		Normal3f n;
		Vector3f dpdu, dpdv;
		Normal3f dndu, dndv;
	}shading;
};
#endif // __DifferentialGeometry__