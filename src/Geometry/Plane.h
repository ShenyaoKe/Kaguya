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
	geoPlane(const Vector3D &pos, const Vector3D &norm);
	~geoPlane();

	void bounding();
	void setOrigin(const Vector3D &pos);
	void setNorm(const Vector3D &norm);

	bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D &pos) const;
	void getNormal(const DifferentialGeometry* queryPoint) const;

	bool isInside(const Vector3D &pPos) const;

public:
	Vector3D n;
};