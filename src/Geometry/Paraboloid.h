#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Shape.h"

/************************************************************************/
/* Paraboloid Function Definition                                       */
/************************************************************************/
typedef enum
{
	ELLIPTIC_PARABOLOID = 1,
	HYPERBOLIC_PARABOLOID = -1
}PARABOLOID_TYPE;
class geoParaboloid :public Shape
{
public:
	geoParaboloid();
	geoParaboloid(const Vector3D &pos, const Float& semiA, const Float& semiB, const Float& semiC, PARABOLOID_TYPE newType);
	~geoParaboloid();

	void setCenter(const Vector3D &pos);
	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	void setParaboloidType(PARABOLOID_TYPE newType);

	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D &pos) const;

	bool isInside(const Vector3D &pPos) const;

public:
	Vector3D c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
	PARABOLOID_TYPE pbType;
};