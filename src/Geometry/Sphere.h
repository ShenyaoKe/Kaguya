/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
#pragma once
#include "Math/MathUtil.h"
#include "Geometry/Shape.h"

class geoSphere :public Shape
{
public:
	geoSphere(const Transform* o2w, const Transform* w2o,
		Float radius = 1., Float phi = M_TWOPI,
		Float th0 = 0, Float th1 = M_PI);
	
	void bounding();
	Bounds3f getWorldBounding() const;
	bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	bool isInside(const Point3f &pPos) const;

private:
	Float r;//radius
	Float phiMax;
	Float thetaMin, thetaMax;
	Float zMin, zMax;
};

