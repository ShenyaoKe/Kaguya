#include "Paraboloid.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

namespace Kaguya
{

/************************************************************************/
/* Paraboloid Function Definition                                       */
/************************************************************************/
geoParaboloid::geoParaboloid()
{
    sa = 1;
    sb = 1;
    sc = 1;
    pbType = ELLIPTIC_PARABOLOID;
}
geoParaboloid::geoParaboloid(const Point3f &pos, Float semiA, Float semiB, Float semiC, PARABOLOID_TYPE newType)
{
    c = pos;
    sa = semiA;
    sb = semiB;
    sc = semiC;
    pbType = newType;
}
geoParaboloid::~geoParaboloid()
{
}
void geoParaboloid::setCenter(const Point3f &pos)
{
    c = pos;
}
void geoParaboloid::setSemiAxes(Float semiA, Float semiB, Float semiC)
{
    sa = semiA;
    sb = semiB;
    sc = semiC;
}
void geoParaboloid::setParaboloidType(PARABOLOID_TYPE newType)
{
    pbType = newType;
}
bool geoParaboloid::intersect(const Ray &inRay, DifferentialGeometry* dg, Float* tHit, Float* rayEpsilon) const
{
    Point3f rp = inRay.o;
    Vector3f rd = inRay.d;// Ray postion and ray direction.	
    Float coeA = (rd.x / sa) * (rd.x / sa) + pbType * (rd.z / sc) * (rd.z / sc);
    Float coeB = (c.x - rp.x) * rd.x / (sa * sa) + pbType * (c.z - rp.z) * rd.z / (sc * sc) + rd.y * 0.5 / sb;
    Float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + pbType * (rp.z - c.z) * (rp.z - c.z) / (sc * sc) + (c.y - rp.y) / sb;
    Float delta = coeB * coeB - coeA * coeC;
    if (coeA == 0)
    {
        inRay.setT(coeC * 0.5 / coeB, coeC * 0.5 / coeB);
        return true;
    }
    else
    {
        if (delta < 0)
        {

            inRay.setT(NAN, NAN);
            //std::cout << "No DifferentialGeometry!" << std::endl;
            return false;
        }
        else//delta > 0
        {
            Float t1 = (coeB - sqrt(delta)) / coeA;
            Float t2 = 2 * coeB / coeA - t1;
            // 		t1 /= coeA;
            // 		t2 /= coeA;
            inRay.setT(t1, t2);

            if (t1 > 0 && t2 > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
/*
Vector3D geoParaboloid::getNormal(const Point3f &pos) const
{
    // 2(x-c.x) / sa^2, -1/sb, 2* type *(z - c.z) / sb^2
    return Normalize(Vector3D(
        2 * (pos.x - c.x) / (sa * sa),
        -1 / sb,
        pbType * 2 * (pos.z - c.z) / (sc * sc)));
}*/
bool geoParaboloid::isInside(const Point3f &pPos) const
{
    return 0;
}

}
