#include "Plane.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

namespace Kaguya
{

/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
geoPlane::geoPlane()
    : n(0., 1., 0.)
{
    bounding();
}
geoPlane::geoPlane(const Point3f &pos, const Normal3f &norm,
                   Float w, Float h)
    : p(pos), n(norm)
    , width(w), height(h)
{
    bounding();
}

bool geoPlane::intersect(const Ray &inRay, DifferentialGeometry* dg, Float* tHit, Float* rayEpsilon) const
{
    Float t = dot(this->n, inRay.d);
    if (t >= 0)
    {
        //std::cout << "Ray parallels to the plane or in the plane." << std::endl;
        return false;
    }
    else
    {
        t = dot(n, (p - inRay.o)) / t;
        if (t > 0 && t > inRay.tmin && t < inRay.tmax)
        {

            //inRay.setT(t, INFINITY);
            *tHit = t;
            *rayEpsilon = reCE * *tHit;
            return true;
        }
        else
        {
            return false;
        }
    }
}

void geoPlane::bounding()
{
    // TODO: Plane bouding method
    ObjBound = Bounds3f(Point3f(-INFINITY, -INFINITY, -INFINITY), Point3f(INFINITY, INFINITY, INFINITY));
}

}
