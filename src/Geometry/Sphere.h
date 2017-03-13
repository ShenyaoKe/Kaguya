/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
#pragma once
#include "Math/MathUtil.h"
#include "Geometry/ParametricGeomtry.h"

namespace Kaguya
{

class geoSphere : public ParametricGeomtry
{
public:
    geoSphere(const Transform* o2w,
              Float radius = 1, Float phi = M_TWOPI,
              Float th0 = 0, Float th1 = M_PI);

    void bounding();
    Bounds3f getWorldBounding() const;
    bool intersect(const Ray &inRay, DifferentialGeometry* dg, Float* tHit, Float* rayEpsilon) const;
    void postIntersect(const Ray &inRay,
                       DifferentialGeometry* dg) const;
    bool isInside(const Point3f &pPos) const;

private:
    Float r;//radius
    Float phiMax;
    Float thetaMin, thetaMax;
    Float zMin, zMax;
};

}
