#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Shape.h"

/************************************************************************/
/* Ellipsoid Function Definition                                        */
/************************************************************************/
class geoEllipsoid :public Shape
{
public:
	geoEllipsoid();
	geoEllipsoid(const Point3f &pos, const Float& semiA, const Float& semiB, const Float& semiC);

	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	bool intersect(const Ray& inRay, DifferentialGeometry* dg, Float *tHit, Float *rayEpsilon) const;
	
	bool isInside(const Point3f &pPos) const;

public:
	Point3f c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
};