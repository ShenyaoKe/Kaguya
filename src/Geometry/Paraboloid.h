#pragma once
#include "Geometry/ParametricGeomtry.h"

/************************************************************************/
/* Paraboloid Function Definition                                       */
/************************************************************************/
enum PARABOLOID_TYPE
{
	ELLIPTIC_PARABOLOID = 1,
	HYPERBOLIC_PARABOLOID = -1
};
class geoParaboloid : public ParametricGeomtry
{
public:
	geoParaboloid();
	geoParaboloid(const Point3f &pos, Float semiA, Float semiB, Float semiC, PARABOLOID_TYPE newType);
	~geoParaboloid();

	void setCenter(const Point3f &pos);
	void setSemiAxes(Float semiA, Float semiB, Float semiC);
	void setParaboloidType(PARABOLOID_TYPE newType);

	bool intersect(const Ray &inRay, DifferentialGeometry* dg, Float* tHit, Float* rayEpsilon) const;
	//Vector3D getNormal(const Point3f &pos) const;

	bool isInside(const Point3f &pPos) const;

public:
	Point3f c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
	PARABOLOID_TYPE pbType;
};