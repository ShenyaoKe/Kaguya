#include "Tracer/Ray.h"

namespace Kaguya
{

Ray::Ray(const Point3f &pos, const Vector3f &dir,
         Float minT, Float maxT)
    : o(pos), d(normalize(dir))
    , tmin(minT), tmax(maxT)
    , time(0), mask(-1)
    , geomID(-1), primID(-1), instID(-1)
    , dp(0)
{
}

Point3f Ray::operator()(Float t) const
{
    return o + d * t;
}

void Ray::setT(Float t1, Float t2) const
{
    tmin = t1;
    tmax = t2;
}

Float Ray::getDifferenceT() const
{
    return tmax - tmin;
}

void Ray::printInfo() const
{
    std::cout << "Origin point position: " << o << std::endl;
    std::cout << "Ray direction: " << d << std::endl;
}

}
