/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
#pragma once
#include "Geometry/Shape.h"

class geoSphere :public Shape
{
public:
	geoSphere(const Vector3D &pos = Vector3D(0, 0, 0), const Float& radius = 1);
	~geoSphere();

	void bounding();
	BBox getWorldBounding() const;
	bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	bool isInside(const Vector3D &pPos) const;

public:
	Float r;//radius
};

