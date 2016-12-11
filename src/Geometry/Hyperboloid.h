#pragma once
#include "Geometry/ParametricGeomtry.h"

/************************************************************************/
/* Hyperboloid Function Definition                                      */
/************************************************************************/
enum HYPERBOLOID_TYPE
{
	ONE_SHEET = 1,
	TWO_SHEET = -1,
	CONE = 0
};

class geoHyperboloid : public ParametricGeomtry
{
public:
	geoHyperboloid();
	geoHyperboloid(const Point3f &pos, Float semiA, Float semiB, Float semiC, HYPERBOLOID_TYPE newType);
	~geoHyperboloid();

	void setCenter(const Point3f &pos);
	void setSemiAxes(Float semiA, Float semiB, Float semiC);
	void setHyperboloidType(HYPERBOLOID_TYPE newType);
	bool intersect(const Ray &inRay, DifferentialGeometry* dg, Float* tHit, Float* rayEpsilon) const;
	//Normal3f getNormal(const Point3f &pos) const;

	bool isInside(const Point3f &pPos) const;

public:
	Point3f c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
	HYPERBOLOID_TYPE hbType;
};