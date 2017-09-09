#pragma once
#include "Geometry/ParametricGeomtry.h"

namespace Kaguya
{

/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
class geoPlane : public ParametricGeomtry
{
public:
	geoPlane();
	geoPlane(const Point3f &pos, const Normal3f &norm,
			 Float w = INFINITY, Float h = INFINITY);

	void bounding();

	bool intersect(const Ray &inRay, Intersection* isec, Float* tHit, Float* rayEpsilon) const;

	//bool isInside(const Point3f &pPos) const;

public:
	Point3f p;
	Normal3f n;
	Float width, height;
};

}
