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
	geoEllipsoid(const Vector3D &pos, const Float& semiA, const Float& semiB, const Float& semiC);
	~geoEllipsoid();

	void setCenter(const Vector3D &pos);
	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D &pos) const;

	bool isInside(const Vector3D &pPos) const;

public:
	Vector3D c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
};