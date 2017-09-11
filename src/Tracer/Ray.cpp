#include "Tracer/Ray.h"
#include "Geometry/Geometry.h"

namespace Kaguya
{

Ray::Ray(const Point3f &pos, const Vector3f &dir,
		 Float minT, Float maxT)
	: o(pos), d(normalize(dir))
	, tMin(minT), tMax(maxT)
	, time(0)
	, mask(sInvalidGeomID)
	, geomID(sInvalidGeomID)
	, primID(sInvalidGeomID)
	, instID(sInvalidGeomID)
	, dp(0)
{
}

Point3f Ray::operator()(Float t) const
{
	return o + d * t;
}

void Ray::setT(Float t1, Float t2) const
{
	tMin = t1;
	tMax = t2;
}

Float Ray::getDifferenceT() const
{
	return tMax - tMin;
}

void Ray::printInfo() const
{
	std::cout << "Origin point position: " << o << std::endl;
	std::cout << "Ray direction: " << d << std::endl;
}

}
