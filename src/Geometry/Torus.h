#pragma once
#include "Geometry/ParametricGeomtry.h"

namespace Kaguya
{

/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
class geoTorus : public ParametricGeomtry
{
public:
    geoTorus() {};
    geoTorus(const Point3f &pos, Float radius, Float secRadius);

    bool intersect(const Ray &inRay, DifferentialGeometry* dg, Float* tHit, Float* rayEpsilon) const;
    bool isInside(const Point3f &pPos) const;

public:
    Point3f c;//center
    Float r = 1, sr = 0.5;// radius and section radius
};

}
