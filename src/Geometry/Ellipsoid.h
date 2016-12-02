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
	geoEllipsoid(const Point3f &pos, Float semiA, Float semiB, Float semiC);

	void setSemiAxes(Float semiA, Float semiB, Float semiC);
	bool intersect(const Ray &inRay, DifferentialGeometry* dg, Float* tHit, Float* rayEpsilon) const;
	
	bool isInside(const Point3f &pPos) const;

public:
	Point3f c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
};