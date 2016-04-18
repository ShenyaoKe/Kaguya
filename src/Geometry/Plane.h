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
	geoPlane(const Point3f &pos, const Normal3f &norm);

	void bounding();

	bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	
	//bool isInside(const Point3f &pPos) const;

public:
	Normal3f n;
};