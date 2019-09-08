#include "Tracer/Ray.h"
#include "Geometry/Geometry.h"

namespace Kaguya
{

Ray::Ray(const Point3f &pos, const Vector3f &dir,
		 Float minT, Float maxT)
	: o(pos)
	, tMin(minT)
	, d(normalize(dir))
	, time(0)
	, tMax(maxT)
	, mask(sInvalidGeomID)
	, mId(0)
	, geomID(sInvalidGeomID)
	, primID(sInvalidGeomID)
	, instID(sInvalidGeomID)
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
