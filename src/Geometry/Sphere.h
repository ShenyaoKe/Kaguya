/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
#pragma once
#include "Geometry/Shape.h"

class geoSphere :public Shape
{
public:
	geoSphere(const Point3f &pos = Point3f(), const Float& radius = 1);
	~geoSphere();

	void bounding();
	BBox getWorldBounding() const;
	bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	bool isInside(const Point3f &pPos) const;

public:
	Float r;//radius
};

