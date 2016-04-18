#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Shape.h"

/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
class geoTorus :public Shape
{
public:
	geoTorus() {};
	geoTorus(const Point3f &pos, const Float& radius, const Float& secRadius);

	bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	bool isInside(const Point3f &pPos) const;

public:
	Point3f c;//center
	Float r = 1, sr = 0.5;// radius and section radius
};
