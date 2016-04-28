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
	geoParaboloid(const Point3f &pos, const Float& semiA, const Float& semiB, const Float& semiC, PARABOLOID_TYPE newType);
	~geoParaboloid();

	void setCenter(const Point3f &pos);
	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	void setParaboloidType(PARABOLOID_TYPE newType);

	bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	//Vector3D getNormal(const Point3f &pos) const;

	bool isInside(const Point3f &pPos) const;

public:
	Point3f c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
	PARABOLOID_TYPE pbType;
};