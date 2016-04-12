#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Shape.h"

/************************************************************************/
/* Hyperboloid Function Definition                                      */
/************************************************************************/
typedef enum
{
	ONE_SHEET = 1,
	TWO_SHEET = -1,
	CONE = 0
}HYPERBOLOID_TYPE;

class geoHyperboloid :public Shape
{
public:
	geoHyperboloid();
	geoHyperboloid(const Vector3D &pos, const Float& semiA, const Float& semiB, const Float& semiC, HYPERBOLOID_TYPE newType);
	~geoHyperboloid();

	void setCenter(const Vector3D &pos);
	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	void setHyperboloidType(HYPERBOLOID_TYPE newType);
	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D &pos) const;

	bool isInside(const Vector3D &pPos) const;

public:
	Vector3D c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
	HYPERBOLOID_TYPE hbType;
};