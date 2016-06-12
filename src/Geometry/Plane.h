#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Shape.h"

/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
class geoPlane :public Shape
{
public:
	geoPlane();
	geoPlane(const Point3f &pos, const Normal3f &norm,
		Float w = INFINITY, Float h = INFINITY);

	void bounding();

	bool intersect(const Ray& inRay, DifferentialGeometry* dg, Float *tHit, Float *rayEpsilon) const;
	
	//bool isInside(const Point3f &pPos) const;

public:
	Point3f p;
	Normal3f n;
	Float width, height;
};